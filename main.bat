set INPUT_DIR=input
set SCRIPT_DIR=scr

python %SCRIPT_DIR%/convert2matrix.py %INPUT_DIR%
start main.exe %INPUT_DIR%