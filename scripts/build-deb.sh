#!/usr/bin/env bash
# Builds the Debian package of WebPassWare: release build, tests, CPack,
# then checks that the unpacked program uses the Qt in the package.
#
#   scripts/build-deb.sh [options]
#
# Options:
#   --no-tests     skip the tests of the release build
#   --clean        start from an empty build/release
#   --no-lintian   skip the lintian summary
#   --output DIR   where to put the .deb (default: build/dist)
#   --jobs N, --qt DIR, --ninja FILE   as for scripts/build.sh
#   -h, --help
set -euo pipefail

usage() {
    # The comment block at the top of this file.
    awk 'NR > 1 && /^#/ { sub(/^# ?/, ""); print; next } NR > 1 { exit }' "$0"
}

die() {
    echo "build-deb.sh: $*" >&2
    exit 1
}

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
output="$root/build/dist"
lintian=1
build_args=(release)

while [[ $# -gt 0 ]]; do
    case "$1" in
        --no-tests | --clean) build_args+=("$1") ;;
        --no-lintian) lintian=0 ;;
        --output) [[ $# -ge 2 ]] || die "--output needs a directory"; output="$2"; shift ;;
        --jobs | --qt | --ninja)
            [[ $# -ge 2 ]] || die "$1 needs a value"
            build_args+=("$1" "$2"); shift ;;
        -h | --help) usage; exit 0 ;;
        *) usage >&2; die "unknown argument: $1" ;;
    esac
    shift
done

for tool in cpack dpkg-deb dpkg-shlibdeps; do
    command -v "$tool" >/dev/null || die "$tool not found (sudo apt install cmake dpkg-dev)"
done

"$root/scripts/build.sh" "${build_args[@]}"

build_dir="$root/build/release"
echo "== package"
rm -f "$build_dir"/webpassware_*.deb
(cd "$build_dir" && cpack -G DEB)
shopt -s nullglob
packages=("$build_dir"/webpassware_*.deb)
[[ ${#packages[@]} -eq 1 ]] || die "expected one package in $build_dir, found ${#packages[@]}"
package="${packages[0]}"

echo "== check: the unpacked program uses the Qt in the package"
check_dir="$(mktemp -d)"
trap 'rm -rf -- "$check_dir"' EXIT
dpkg-deb -x "$package" "$check_dir"
prefix="$check_dir/opt/webpassware"
missing="$(ldd "$prefix/bin/webpassware" | grep 'not found' || true)"
[[ -z "$missing" ]] || die "missing libraries: $missing"
outside="$(ldd "$prefix/bin/webpassware" | grep 'libQt6' | grep -v "$prefix" || true)"
[[ -z "$outside" ]] || die "Qt libraries from outside the package: $outside"
"$prefix/bin/webpassware" --version
for plugin in sqldrivers/libqsqlite.so platforms/libqxcb.so; do
    [[ -f "$prefix/plugins/$plugin" ]] || die "plugin missing from the package: $plugin"
done

if [[ $lintian -eq 1 ]] && command -v lintian >/dev/null; then
    echo "== lintian (expected for a self-contained package in /opt: dir-or-file-in-opt, embedded Qt/ICU)"
    lintian "$package" 2>&1 | grep -E '^[EW]:' | cut -d' ' -f1,3 | sort | uniq -c | sort -rn || true
fi

mkdir -p "$output"
cp -f "$package" "$output/"
echo "== done"
dpkg-deb -f "$package" Package Version Architecture Installed-Size
echo "Package: $output/$(basename "$package") ($(du -h "$package" | cut -f1))"
echo "Install: sudo apt install $output/$(basename "$package")"
