# SimpleOS
As its name implies, SimpleOS is an operating system focused on simplicity.
## Building
### Preparation
You'll need to install the dependencies required for building:
```console
sudo apt install build-essential bison flex grub-pc-bin libgmp3-dev libmpc-dev libmpfr-dev mtools texinfo xorriso
```
Ater that, clone this repository:
```console
git clone https://github.com/JustSomeCoder07/SimpleOS
```
To build the cross-compiler, you'll need the newest version of GCC installed.
If you don't have GCC installed or if it's not the newest version available, 
run the following script to run it:
```console
tools/build-compiler.sh
```
You'll also need to add the directory `$HOME/opt/gcc/bin` to your `$PATH`environment variable.
To permanently add the directory to your path, please refer to your shell manufacturer's documentation.
To add it for the current shell session, simply run the following command:
```console
export PATH="$HOME/opt/gcc/bin:$PATH"
```
After that, you're ready to build and install the cross-compiler:
```console
tools/build-cross-compiler.sh
```
You'll also need to add the directory `$HOME/opt/cross/bin` to your `$PATH`environment variable.
To permanently add the directory to your path, please refer to your shell manufacturer's documentation.
To add it for the current shell session, simply run the following command:
```console
export PATH="$HOME/opt/cross/bin:$PATH"
```
You're now ready to build SimpleOS!
### Building SimpleOS
If you want to test SimpleOS, you'll need to install Qemu:
```console
sudo apt install qemu qemu-system-i386
```
You can now use the shell scripts in the root directory. <br>
Building headers: `headers.sh` <br>
Building ISO: `iso.sh` <br>
Building and running: `qemu.sh` <br>
Cleaning: `clean.sh`
