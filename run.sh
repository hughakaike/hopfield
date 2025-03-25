#!/bin/bash
SCRIPT_DIR="scr"

python $SCRIPT_DIR/convert2matrix.py
./main.exe
python $SCRIPT_DIR/convert2image.py
