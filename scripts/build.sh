#!/usr/bin/env bash
# Configures, builds and tests WebPassWare with the CMake presets.
#
#   scripts/build.sh [debug|release|coverage] [options]
#
# Options:
#   --no-tests     build only
#   --clean        remove the build directory first
#   --jobs N       parallel jobs (default: number of CPUs)
#   --qt DIR       Qt installation (default: $QT_DIR or /opt/Qt/6.12.0/gcc_64)
#   --ninja FILE   Ninja (default: $NINJA, /opt/Qt/Tools/Ninja/ninja or ninja on PATH)
#   -h, --help
set -euo pipefail

usage() {
    # The comment block at the top of this file.
    awk 'NR > 1 && /^#/ { sub(/^# ?/, ""); print; next } NR > 1 { exit }' "$0"
}

die() {
    echo "build.sh: $*" >&2
    exit 1
}

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
type="debug"
tests=1
clean=0
jobs="$(nproc 2>/dev/null || echo 4)"
qt="${QT_DIR:-/opt/Qt/6.12.0/gcc_64}"
ninja="${NINJA:-}"

while [[ $# -gt 0 ]]; do
    case "$1" in
        debug | release | coverage) type="$1" ;;
        --no-tests) tests=0 ;;
        --clean) clean=1 ;;
        --jobs) [[ $# -ge 2 ]] || die "--jobs needs a number"; jobs="$2"; shift ;;
        --qt) [[ $# -ge 2 ]] || die "--qt needs a directory"; qt="$2"; shift ;;
        --ninja) [[ $# -ge 2 ]] || die "--ninja needs a file"; ninja="$2"; shift ;;
        -h | --help) usage; exit 0 ;;
        *) usage >&2; die "unknown argument: $1" ;;
    esac
    shift
done

[[ "$jobs" =~ ^[0-9]+$ && "$jobs" -gt 0 ]] || die "--jobs must be a positive number"
command -v cmake >/dev/null || die "cmake not found (3.21 or newer)"
[[ -f "$qt/lib/cmake/Qt6/Qt6Config.cmake" ]] || die "no Qt 6 in $qt (give --qt or QT_DIR)"
if [[ -z "$ninja" ]]; then
    if [[ -x /opt/Qt/Tools/Ninja/ninja ]]; then
        ninja=/opt/Qt/Tools/Ninja/ninja
    else
        ninja="$(command -v ninja || true)"
    fi
fi
[[ -n "$ninja" && -x "$ninja" ]] || die "ninja not found (give --ninja or NINJA)"

build_dir="$root/build/$type"
if [[ $clean -eq 1 && -d "$build_dir" ]]; then
    echo "== removing $build_dir"
    rm -rf -- "$build_dir"
fi

cd "$root"
echo "== configure ($type, Qt $qt)"
cmake --preset "$type" -DCMAKE_PREFIX_PATH="$qt" -DCMAKE_MAKE_PROGRAM="$ninja"

echo "== build ($jobs jobs)"
cmake --build --preset "$type" --parallel "$jobs"

if [[ $tests -eq 1 ]]; then
    echo "== tests"
    ctest --preset "$type" --parallel "$jobs"
fi

echo "== done"
echo "Program: $build_dir/webpassware"
