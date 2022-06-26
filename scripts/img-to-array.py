#!/bin/python

from sys import argv
from PIL import Image

img = Image.open(argv[1]).convert('RGB')

tw = img.width  // 8
th = img.height // 8

output = ''

palette = {}

for tile in range(tw * th):
    for y in range(8):
        output += '\n    '
        for x in range(8):

            pix = img.getpixel(( (tile % tw) * 8 + x, (tile // tw) * 8 + y ))
            pix = pix[0] << 16 | pix[1] << 8 | pix[2];

            if pix not in palette:
                palette[pix] = int(input('color ' + hex(pix)[2:].zfill(6) + ': '), 16)

            if x & 1 == 0:
                output += '0x'

            output += hex(palette[pix])[2:]

            if x & 1 == 1:
                output += ', '
    output += '\n'

print('---PRINTING OUTPUT---')
print(output)
