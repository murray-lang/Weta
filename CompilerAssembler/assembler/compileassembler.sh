#!/bin/sh
#cd lib
#./compile.sh
#cd ..
nodejs ../../../../node_modules/jscc-node/jscc.js -t "../../../../node_modules/jscc-node/driver_node.js_" -p WasmCC -o WasmParser.js WasmParser.par
