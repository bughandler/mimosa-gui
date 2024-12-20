#!/bin/bash -e

VERSION=0.15.3

cd thirdparty/thorvg/ || true
rm -rf AUTHORS LICENSE inc/ src/ *.zip *.tar.gz tmp/

mkdir tmp/ && pushd tmp/

# Release
curl -L -O https://github.com/thorvg/thorvg/archive/v$VERSION.tar.gz
# Current Github main branch tip
#curl -L -O https://github.com/thorvg/thorvg/archive/refs/heads/main.tar.gz

tar --strip-components=1 -xvf *.tar.gz
rm *.tar.gz

# Install from local git checkout "thorvg-git" in the same directory
# as godot git checkout.
#d="../../../../thorvg-git"
#cp -r ${d}/AUTHORS ${d}/inc ${d}/LICENSE ${d}/src .

find2 . -type f -name 'meson.build' -delete

# Fix newline at end of file.
for source in $(find ./ -type f \( -iname \*.h -o -iname \*.cpp \)); do
    sed -i -e '$a\' $source
done

cp -v AUTHORS LICENSE ..
cp -rv inc ../

cat << EOF > ../inc/config.h
#ifndef THORVG_CONFIG_H
#define THORVG_CONFIG_H

#define THORVG_SW_RASTER_SUPPORT
#define THORVG_SVG_LOADER_SUPPORT
#define THORVG_JPG_LOADER_SUPPORT
#ifdef LOTTIE_ENABLED
#define THORVG_LOTTIE_LOADER_SUPPORT
#endif //LOTTIE_ENABLED
#define THORVG_THREAD_SUPPORT

// Added conditionally if webp module is enabled.
//#define THORVG_WEBP_LOADER_SUPPORT

// For internal debugging:
//#define THORVG_LOG_ENABLED

#define THORVG_VERSION_STRING "$VERSION"
#endif
EOF

mkdir ../src
cp -rv src/common ../src
cp -rv src/renderer ../src/

# Only sw_engine is enabled.
rm -rfv ../src/renderer/gl_engine
rm -rfv ../src/renderer/wg_engine

# Enabled embedded loaders: raw, JPEG
mkdir ../src/loaders
cp -rv src/loaders/svg src/loaders/raw  ../src/loaders/
cp -rv src/loaders/lottie ../src/loaders/
# Not using external jpg as it's turbojpeg, which we don't have.
cp -rv src/loaders/jpg ../src/loaders/

popd
rm -rf tmp

