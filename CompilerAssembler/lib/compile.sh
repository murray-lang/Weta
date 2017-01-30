#!/bin/sh
cd config
./compileconfig.sh
cd ..
cd math
./compilemath.sh
cd ..
cd comm
./compilecomm.sh
cd ..

