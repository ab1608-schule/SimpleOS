#!/bin/sh

# Variables
export GCC_VERSION="13.2.0"
export GDB_VERSION="13.2"
export BINUTILS_VERSION="2.41"
export PREFIX="$HOME/opt/cross"
export TARGET="i686-elf"
export PATH="$PREFIX/bin:$PATH"

# Preparation
rm -rf tmp
mkdir tmp
cd tmp

# Build Binutils
wget ftp://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.xz
tar -xvf binutils-$BINUTILS_VERSION.tar.xz
mkdir build-binutils
cd build-binutils
../binutils-$BINUTILS_VERSION/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j$(($(nproc) + 1))
make install -j$(($(nproc) + 1))
cd ../

# Build GDB
wget ftp://ftp.gnu.org/gnu/gdb/gdb-$GDB_VERSION.tar.xz
tar -xvf gdb-$GDB_VERSION.tar.xz
mkdir build-gdb
cd build-gdb
../gdb-$GDB_VERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-werror
make all-gdb -j$(($(nproc) + 1))
make install-gdb -j$(($(nproc) + 1))
cd ../

# Build GCC
wget ftp://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.xz
tar -xvf gcc-$GCC_VERSION.tar.xz
mkdir build-gcc
cd build-gcc
../gcc-$GCC_VERSION/contrib/download_prerequisites
../gcc-$GCC_VERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j$(($(nproc) + 1))
make all-target-libgcc -j$(($(nproc) + 1))
make install-gcc
make install-target-libgcc
cd ../

# Cleanup
cd ../
rm -rf tmp