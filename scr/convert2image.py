# about: converts an image to matrix data (cmatrix.h format)
# usage: python convert2image.py hoge.dat > image

# requires PIL or Pillow library

import sys
import os
import numpy as np

from PIL import Image, ImageOps

path=sys.argv[1]

image_gray=np.loadtxt(sys.argv[1],skiprows=1)

print(image_gray.shape)

pil_img = Image.fromarray(image_gray)
pil_img = pil_img.convert("L")

pil_img.save('SVD_image.png')
