#!/bin/bash

USAGE() {
  echo "Usage: ./make.bash (terminal, sapp, sapp-em) (debug(default), release)"
  exit -1
}

RUN() {
  COMMAND=$1
  echo ${COMMAND}
  ${COMMAND}
}

if [ $# -lt 1 ]; then
  USAGE
fi

if [ $# -eq 2 ]; then
  if [ $2 == "debug" ] || [ $2 == "release" ]; then
    MODE=$2
  else
    USAGE
  fi
fi

PROG='jcw'
COMPILER='gcc'
C_FLAGS="-std=c99 -Wall -Wextra -fmax-errors=5 -Wno-unused-parameter -Wno-missing-field-initializers -o ${PROG}"

if [ "${MODE}" == "release" ] || [ $1 == "sapp-em" ] || [ $1 == "sgl-sapp-em" ]; then
  C_FLAGS="-O3 -s ${C_FLAGS}"
  echo "Compiling (release)"
else
  echo "Compiling (debug)"
fi


if [ $1 == "terminal" ]; then
  RUN "${COMPILER} ${C_FLAGS} main-terminal.c"

elif [ $1 == "sapp" ]; then
  RUN "${COMPILER} ${C_FLAGS} -I./sokol sokol/sokol.c main-sapp.c -lX11 -lXi -lXcursor -lGL -ldl -lm -lpthread"

elif [ $1 == "sapp-em" ]; then
  RUN "emcc -Os -DNDEBUG -std=gnu11 -o sokol.c.o -c sokol/sokol.c"
  RUN "emar qc libsokol.a sokol.c.o"
  RUN "emranlib libsokol.a"
  RUN "emcc -I./sokol -Os -DNDEBUG -std=gnu11 -o main-sapp.c.o -c main-sapp.c"
  RUN "emcc -Os -DNDEBUG --shell-file sokol/shell.html -sNO_FILESYSTEM=1 -sASSERTIONS=0 -sMALLOC=emmalloc --closure=1 main-sapp.c.o -o jcw.html libsokol.a"
  RUN "mv jcw.html docs/index.html"
  RUN "mv jcw.js docs/"
  RUN "mv jcw.wasm docs/"
  # emrun docs/index.html

else
  USAGE
fi
