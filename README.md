# A small command-line utility to visualize sorting algorithms.

## Install

### Dependencies

It is necessary to install the raylib dependencies.

#### Using APT :

`sudo apt-get install build-essential git libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxext-dev libgl1-mesa-dev`

#### Building

After opening a terminal in the directory of the project, you can build the the project :\
`make`

Add the resulting binary to the user's bin folder :\
`make install`

You can remove the binary from the user's bin folder :\
`make uninstall`

Additional options such as **clean**, **rebuild** and **reinstall** are also available in the Makefile.

## Usage :

```
Usage: sortviz [OPTIONS]
  Options:
    --bubble, -b
    --selection, -s
    --insertion, -i
    --quick, -q
    --merge, -m
    --gui <do the sort in a dedicated window>
    -size <size of the array (only accounted for in the gui mode)>
  Example: sortviz --bubble
           sortviz -b --gui
```
