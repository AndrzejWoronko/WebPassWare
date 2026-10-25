# WebPassWare

Application for managing passwords from websites, e-mail accounts, social accounts, etc.
It has a password generator, password groups, easy filtering.
Additionally, it has an internet update.

![image](https://github.com/AndrzejWoronko/WebPassWare/blob/master/icons/50px/WebPassWare.png)

# Screenshots

## DB pass
![image](https://github.com/AndrzejWoronko/WebPassWare/blob/master/screenshots/Db_pass.png)

## Mainwindow
![image](https://github.com/AndrzejWoronko/WebPassWare/blob/master/screenshots/Mainwindow.png)

## Entry add/edit
![image](https://github.com/AndrzejWoronko/WebPassWare/blob/master/screenshots/Entry_edit.png)

# Build

Qt 6.12, CMake 3.21+, Ninja and a C++17 compiler. On Linux auto-type also needs the
X11 and XTest development files (`sudo apt install libx11-dev libxtst-dev`).

```bash
scripts/build.sh                  # debug: configure, build, run the tests
scripts/build.sh release --clean
scripts/build.sh --qt ~/Qt/6.12.0/gcc_64   # another Qt installation
```

The program is `build/<preset>/webpassware`. The presets in `CMakePresets.json` expect
Qt in `/opt/Qt/6.12.0/gcc_64` and Ninja in `/opt/Qt/Tools/Ninja/ninja`.

# Tests

```bash
ctest --preset debug
```

The tests run offscreen; `tst_autotype` needs an X11 display.

# Packages

- Debian/Ubuntu: `scripts/build-deb.sh` builds `build/dist/webpassware_<version>_amd64.deb`
  with Qt included (installed into `/opt/webpassware`, command `webpassware`).
- Windows: `scripts/windows/build-portable.sh` (Git Bash, Qt for MinGW, windeployqt).
- macOS: `scripts/macosx/build-portable.sh` (macdeployqt).
