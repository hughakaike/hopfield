# about: converts an image to matrix data (cmatrix.h format)
# usage: python convert2image.py hoge.dat > image

# requires PIL or Pillow library

import sys
import os
import glob
import numpy as np

from PIL import Image, ImageOps

CDIR=os.getcwd()

path=sys.argv[1]
os.chdir(os.path.join(CDIR,path,"matrix"))

image_files = glob.glob("*.dat")

if len(image_files) == 0:
    print("No data files found.")
    sys.exit(1)

try:
    os.chdir(os.path.join(CDIR,path,"images"))
except:
    os.mkdir(os.path.join(CDIR,path,"images"))
    os.chdir(os.path.join(CDIR,path,"images"))

print(image_files)

for file in image_files:
    image_gray=np.loadtxt(os.path.join(CDIR,path,"matrix",file),skiprows=1)
    pil_img = Image.fromarray(image_gray*255)
    pil_img = pil_img.convert("L")
    pil_img.save(file+".png")