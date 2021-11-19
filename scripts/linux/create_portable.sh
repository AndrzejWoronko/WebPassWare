#!/bin/bash

set -x

QMAKE="/opt/Qt/5.15.2/gcc_64/bin/qmake"

which chrpath >/dev/null
if [ "$?" -ne 0 ]; then
  echo "chrpath program missing! Maybe run sudo apt-get install chrpath first."
  exit 1
fi

qt_paths_bin="${QMAKE/qmake/qtpaths}"
$qt_paths_bin -v >/dev/null 2>&1
if [ "$?" -ne 0 ]; then
  echo "qtpaths program missing!"
  exit 1
fi

OUTPUT=`pwd`/../../output

cd $OUTPUT

required_modules="libQt5Core.so libQt5Gui.so libQt5Widgets.so libQt5Svg.so libQt5XcbQpa.so libQt5XmlPatterns.so libQt5Qml.so libQt5Sql.so"
  

required_plugins="platforms/libqxcb.so platforms/libqlinuxfb.so platforms/libqminimalegl.so platforms/libqminimal.so platforms/libqwebgl.so platforms/libqeglfs.so \
   imageformats/libqgif.so imageformats/libqicns.so imageformats/libqico.so imageformats/libqjpeg.so \
   imageformats/libqsvg.so imageformats/libqtga.so imageformats/libqtiff.so imageformats/libqwbmp.so imageformats/libqwebp.so \
   iconengines/libqsvgicon.so sqldrivers/libqsqlite.so"

optional_plugins="platformthemes/libqgtk3.so platformthemes/libqxdgdesktopportal.so"

required_dirs="libexec resources translations"

required_programs="qtdiag"

qt_lib_dir=`ldd build/bin/webpassware | grep libQt5Core | awk '{print $3;}'`
qt_lib_dir=`dirname $qt_lib_dir`
qt_plugins_dir=`$qt_paths_bin --plugin-dir`
qt_dir=`$qt_paths_bin --install-prefix`

# Create portable dir to store distribution in
rm -rf portable
mkdir portable
cd portable
portable=`pwd`

# Copy all output from compilation here
mkdir -p WebPassWare

mkdir -p WebPassWare/lib/
mkdir -p WebPassWare/lib/Qt/lib
mkdir -p WebPassWare/lib/Qt/bin
mkdir -p WebPassWare/lib/Qt/plugins
mkdir -p WebPassWare/lib/Qt/qml

cp -R $OUTPUT/build/bin/ WebPassWare/

# Make lib directory to move all *.so files (webpassware files and Qt files and dependencies)

cd WebPassWare

# Copy Qt
cd $portable/WebPassWare/lib/Qt/lib/
for module in $required_modules; do
  if [ ! -f $qt_lib_dir/$module ]; then
    echo "Required Qt module doesn't exist: $qt_lib_dir/$module"
    exit 1
  fi
  cp -P $qt_lib_dir/$module* .
#Delete debug
  rm -f $module*.debug
  
  for dep_lib in `ldd $qt_lib_dir/$module | grep $qt_lib_dir | awk '{print $3;}'`; do
   cp -Pu $dep_lib* .
#Delete debug    
   rm -f *.debug
  done
done

for lib in `ls *.so`; do
  chrpath -r \$ORIGIN/ $lib 2>&1 >/dev/null
done

# Copy Qt dirs
cd $portable/WebPassWare/lib/Qt/
for qdir in $required_dirs; do
  if [ ! -d $qt_dir/$qdir ]; then
    echo "Required Qt dir doesn't exist: $qt_dir/$qdir"
    exit 1
  fi
  cp -R $qt_dir/$qdir .
done

# Copy Qt programs
cd $portable/WebPassWare/lib/Qt/bin/
for program in $required_programs; do
  if [ ! -f $qt_dir/bin/$program ]; then
    echo "Required Qt program doesn't exist: $qt_dir/bin/$program"
    exit 1
  fi
  cp -P $qt_dir/bin/$program .
done

# Now copy Qt plugins
cd $portable/WebPassWare/lib/Qt/plugins/
qt_plugin_dirs=()
for plugin in $required_plugins; do
  if [ ! -f $qt_plugins_dir/$plugin ]; then
    echo "Required Qt plugin doesn't exist: $qt_plugins_dir/$plugin"
    exit 1
  fi
  parts=(${plugin/\// })
  mkdir ${parts[0]} 2>/dev/null
  cp -P $qt_plugins_dir/$plugin ${parts[0]}
  
  # Update rpath in Qt plugins
  cd ${parts[0]}
  for lib in `ls *.so`; do
    chrpath -r \$ORIGIN/../../lib $lib 2>&1 >/dev/null
  done
  cd ..
done

for plugin in $optional_plugins; do
  if [ ! -f $qt_plugins_dir/$plugin ]; then
    continue
  fi
  parts=(${plugin/\// })
  mkdir ${parts[0]} 2>/dev/null
  cp -P $qt_plugins_dir/$plugin ${parts[0]}
  
  # Update rpath in Qt plugins
  cd ${parts[0]}
  for lib in `ls *.so`; do
    chrpath -r \$ORIGIN/../../lib $lib 2>&1 >/dev/null
  done
  cd ..
done

cd $portable/WebPassWare/bin/

chrpath -r \$ORIGIN/../lib:\$ORIGIN/../lib/Qt/lib webpassware 2>&1 >/dev/null

cd $portable

VERSION="0.0.1"

# Complete
echo "Building complete package: WebPassWare-$VERSION.tar.xz"
tar cf WebPassWare-$VERSION.tar WebPassWare
xz -z  WebPassWare-$VERSION.tar
  
echo "Done."

echo "Portable distribution created at: $portable/WebPassWare"
