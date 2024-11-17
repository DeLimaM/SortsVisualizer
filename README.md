# A small command-line utility to visualize sorting algorithms.

## Install

After opening a terminal in the directory of the project, you can build the the project :\
`make`

Add the resulting binary to the user's bin folder :\
`make install`

You can remove the binary from the user's bin folder :\
`make uninstall`

Additional options such as **clean**, **rebuild** and **reinstall** are also available in the Makefile.

## Usage :

```Usage: sortviz [OPTIONS]
  Options:
    --bubble, -b
    --selection, -s
    --insertion, -i
    --drunk, -d
    --quick, -q
    --merge, -m
    --stime <value>
  Example: sortviz --bubble --stime 50
```
