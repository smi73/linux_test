#!/bin/bash

CFLAGS="-Os -ffunction-sections -fdata-sections -fno-unwind-tables -fno-asynchronous-unwind-tables -flto"
LDFLAGS="-Wl,-s -Wl,-Bsymbolic -Wl,--gc-sections"
FLAGS_JO=""
TRIPLE="mips-linux-musl"

set -exo pipefail
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd "$DIR"

SRC_JO="https://github.com/jpmens/jo/releases/download/1.4/jo-1.4.tar.gz"

DL_DIR="$(pwd)/jo_src"

mkdir -p "$DL_DIR"
if [ ! -f "$DL_DIR/jo.tar.bz2" ]; then
  wget -O "$DL_DIR/jo.tar.bz2" "$SRC_JO"
fi

work=$(buildah from "localhost/mips_gcc")
function cleanup {
  buildah rm "$work"
}
trap cleanup EXIT

# Need a few things for unpacking and building
buildah run "$work" -- apk add --no-cache tar bzip2 bash libtool patch

# Copy and extract
buildah run "$work" -- mkdir -p /tmp/work/jo
buildah run --volume "$DL_DIR:/tmp/jo_src/" "$work" -- tar -axf "/tmp/jo_src/jo.tar.bz2" -C /tmp/work/jo --strip 1

# Jo build
buildah run "$work" -- /bin/sh -c \
  "cd /tmp/work/jo && CFLAGS='$CFLAGS' LDFLAGS='$LDFLAGS' ./configure --host=$TRIPLE && make all"

#buildah run "$work" -- strip /tmp/work/jo/src/jo

# Grab the built JO binary
buildah run --volume "$(pwd):/output" "$work" -- cp /tmp/work/jo/jo /output/
