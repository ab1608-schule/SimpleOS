#!/bin/sh
set -e
. ./config.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && $MAKE clean -j$(($(nproc) + 1)))
done

rm -rf sysroot
rm -rf isodir
rm -rf simpleos.iso
