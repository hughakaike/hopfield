set SCRIPT_DIR=scr

echo convert image to matrix
python %SCRIPT_DIR%/convert2matrix.py
pause

echo start hopfield main
start main.exe
pause

echo convert matrix to image
python %SCRIPT_DIR%/convert2image.py
