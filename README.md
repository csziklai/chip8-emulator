# Chip8-emulator
This is a Chip-8 emulator designed to emulate the functionality of the Chip8
system.
## How to run
The following instructions are for macOS.
First install glut and XQuartz.
```
brew install --cask xquartz
brew install freeglut
```
Then make the file
```
cd build
make
```
Run the program as `./chip8 <name-of-prog>.ch8`. For example,
```
./chip8 ibm.ch8
```
Sample Chip8 programs can be found at this [repo](https://github.com/kripod/chip8-roms/tree/master). Programs may have to be renamed so that there are no 
spaces in the file name. Note that this emulator only supports modern behavior (as opposed to older games from the 1970s/1980s).