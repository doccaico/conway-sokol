## jcw

Conway's Game of Life in C with [Sokol](https://github.com/floooh/sokol)

## Build

```sh
# terminal
$ ./make.bash terminal

# desktop (sapp)
$ ./make.bash sapp
# desktop (sgl-sapp)
$ ./make.bash sgl-sapp

# emscripten (sapp)
$ ./make.bash sapp-em
# emscripten (sgl-sapp)
$ ./make.bash sgl-sapp-em

# run
./jcw
# run (emscripten)
./emrun jcw.html
```

### main-sapp.c
[WASM version](https://doccaico.github.io/jcw/)

### main-terminal.c
<img src="https://github.com/doccaico/jcw/blob/main/img/terminal.png?raw=true" width="200px">

### main-sgl-sapp.c
<img src="https://github.com/doccaico/jcw/blob/main/img/sgl-sapp.png?raw=true" width="200px">
