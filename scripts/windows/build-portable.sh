#!/usr/bin/env bash
# Builds the portable Windows package of WebPassWare (Git Bash or MSYS2):
# release build with MinGW and Ninja, windeployqt, WebPassWare-<version>.tgz.
#
#   scripts/windows/build-portable.sh [options]
#
# Options:
#   --qt DIR       Qt for MinGW (default: $QT_DIR or /c/Qt/6.12.0/mingw_64)
#   --mingw DIR    MinGW toolchain (default: $MINGW_DIR or the newest /c/Qt/Tools/mingw*_64)
#   --ninja FILE   Ninja (default: $NINJA, /c/Qt/Tools/Ninja/ninja.exe or ninja on PATH)
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
qt="${QT_DIR:-/c/Qt/6.12.0/mingw_64}"
mingw="${MINGW_DIR:-}"
ninja="${NINJA:-}"
clean=0

while [[ $# -gt 0 ]]; do
    case "$1" in
        --qt) [[ $# -ge 2 ]] || die "--qt needs a directory"; qt="$2"; shift ;;
        --mingw) [[ $# -ge 2 ]] || die "--mingw needs a directory"; mingw="$2"; shift ;;
        --ninja) [[ $# -ge 2 ]] || die "--ninja needs a file"; ninja="$2"; shift ;;
        --clean) clean=1 ;;
        -h | --help) usage; exit 0 ;;
        *) usage >&2; die "unknown argument: $1" ;;
    esac
    shift
done

[[ -f "$qt/lib/cmake/Qt6/Qt6Config.cmake" ]] || die "no Qt 6 in $qt (give --qt or QT_DIR)"
if [[ -z "$mingw" ]]; then
    mingw="$(ls -d /c/Qt/Tools/mingw*_64 2>/dev/null | sort -V | tail -1 || true)"
fi
[[ -x "$mingw/bin/g++.exe" ]] || die "no MinGW in '$mingw' (give --mingw or MINGW_DIR)"
if [[ -z "$ninja" ]]; then
    if [[ -x /c/Qt/Tools/Ninja/ninja.exe ]]; then
        ninja=/c/Qt/Tools/Ninja/ninja.exe
    else
        ninja="$(command -v ninja || true)"
    fi
fi
[[ -n "$ninja" && -x "$ninja" ]] || die "ninja not found (give --ninja or NINJA)"
command -v cmake >/dev/null || die "cmake not found (3.21 or newer)"
export PATH="$qt/bin:$mingw/bin:$PATH"

build_dir="$root/build/windows-release"
if [[ $clean -eq 1 && -d "$build_dir" ]]; then
    echo "== removing $build_dir"
    rm -rf -- "$build_dir"
fi

echo "== configure (Qt $qt, MinGW $mingw)"
cmake -S "$root" -B "$build_dir" -G Ninja -DCMAKE_BUILD_TYPE=Release -DWPW_BUILD_TESTS=OFF \
    -DCMAKE_PREFIX_PATH="$qt" -DCMAKE_MAKE_PROGRAM="$ninja" -DCMAKE_CXX_COMPILER="$mingw/bin/g++.exe"
echo "== build"
cmake --build "$build_dir" --parallel

version="$(cut -d '=' -f 2 "$root/CURRENT_VERSION.TXT")"
dist="$root/build/dist"
portable="$dist/WebPassWare"
echo "== deploy into $portable"
rm -rf -- "$portable"
mkdir -p "$portable/bin"
cp "$build_dir/webpassware.exe" "$portable/bin/"
cp "$root/packaging/webpassware.ico" "$root/packaging/webpassware.png" "$portable/"
windeployqt.exe --release --plugindir "$portable/bin/plugins" "$portable/bin/webpassware.exe"

archive="WebPassWare-$version.tgz"
(cd "$dist" && tar czf "$archive" WebPassWare)
echo "== done"
echo "Package: $dist/$archive"
