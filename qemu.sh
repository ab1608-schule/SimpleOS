#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom simpleos.iso

. ./clean.sh
. ./headers.sh