#!/usr/bin/env bash
# Builds the portable macOS package of WebPassWare: release build with Ninja,
# macdeployqt on webpassware.app, WebPassWare-<version>.tgz.
#
#   scripts/macosx/build-portable.sh [options]
#
# Options:
#   --qt DIR       Qt for macOS (default: $QT_DIR or ~/Qt/6.12.0/macos)
#   --ninja FILE   Ninja (default: $NINJA, ~/Qt/Tools/Ninja/ninja or ninja on PATH)
#   --clean        remove the build directory first
#   -h, --help
set -euo pipefail

usage() {
    # The comment block at the top of this file.
    awk 'NR > 1 && /^#/ { sub(/^# ?/, ""); print; next } NR > 1 { exit }' "$0"
}

die() {
    echo "build-portable.sh: $*" >&2
    exit 1
}

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
qt="${QT_DIR:-$HOME/Qt/6.12.0/macos}"
ninja="${NINJA:-}"
clean=0

while [[ $# -gt 0 ]]; do
    case "$1" in
        --qt) [[ $# -ge 2 ]] || die "--qt needs a directory"; qt="$2"; shift ;;
        --ninja) [[ $# -ge 2 ]] || die "--ninja needs a file"; ninja="$2"; shift ;;
        --clean) clean=1 ;;
        -h | --help) usage; exit 0 ;;
        *) usage >&2; die "unknown argument: $1" ;;
    esac
    shift
done

[[ -f "$qt/lib/cmake/Qt6/Qt6Config.cmake" ]] || die "no Qt 6 in $qt (give --qt or QT_DIR)"
if [[ -z "$ninja" ]]; then
    if [[ -x "$HOME/Qt/Tools/Ninja/ninja" ]]; then
        ninja="$HOME/Qt/Tools/Ninja/ninja"
    else
        ninja="$(command -v ninja || true)"
    fi
fi
[[ -n "$ninja" && -x "$ninja" ]] || die "ninja not found (give --ninja or NINJA)"
command -v cmake >/dev/null || die "cmake not found (3.21 or newer)"

build_dir="$root/build/macos-release"
if [[ $clean -eq 1 && -d "$build_dir" ]]; then
    echo "== removing $build_dir"
    rm -rf -- "$build_dir"
fi

echo "== configure (Qt $qt)"
cmake -S "$root" -B "$build_dir" -G Ninja -DCMAKE_BUILD_TYPE=Release -DWPW_BUILD_TESTS=OFF \
    -DCMAKE_PREFIX_PATH="$qt" -DCMAKE_MAKE_PROGRAM="$ninja"
echo "== build"
cmake --build "$build_dir" --parallel

version="$(cut -d '=' -f 2 "$root/CURRENT_VERSION.TXT")"
dist="$root/build/dist"
portable="$dist/WebPassWare"
echo "== deploy into $portable"
rm -rf -- "$portable"
mkdir -p "$portable"
cp -R "$build_dir/webpassware.app" "$portable/"
"$qt/bin/macdeployqt" "$portable/webpassware.app"

archive="WebPassWare-$version.tgz"
(cd "$dist" && tar czf "$archive" WebPassWare)
echo "== done"
echo "Package: $dist/$archive"
