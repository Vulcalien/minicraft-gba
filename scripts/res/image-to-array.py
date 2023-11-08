#!/bin/python

# Copyright 2023 Vulcalien
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import sys, argparse
from sys import exit
from PIL import Image

# Setup argparse
parser = argparse.ArgumentParser(
    description='Convert an image file into a GBA image array'
)

parser.add_argument('-i', '--input',
                    type=argparse.FileType('rb'), required=True,
                    help='specify the filename of the image file')
parser.add_argument('-n', '--name',
                    required=True,
                    help='specify the name of the output array')
parser.add_argument('--bpp',
                    type=int, choices=[4, 8, 16], required=True,
                    help='specify the bits per pixel')

parser.add_argument('--palette',
                    type=argparse.FileType('rb'),
                    help='specify the filename of the palette file')

parser.add_argument('-o', '--output',
                    type=argparse.FileType('w'), default=sys.stdout,
                    help='specify the output file (default: stdout)')
parser.add_argument('-s', '--static',
                    action=argparse.BooleanOptionalAction,
                    help='add the \'static\' modifier to the output ' +
                         'array')

args = parser.parse_args()

# Open image
img = Image.open(args.input).convert('RGB')

# Map the colors by reading the palette image
if args.bpp in [4, 8]:
    color_map = {}

    palette_img = Image.open(args.palette).convert('RGB')
    for y in range(palette_img.height):
        for x in range(palette_img.width):
            pix = palette_img.getpixel( (x, y) )

            if pix not in color_map:
                i = (x + y * palette_img.width) % (2 ** args.bpp)
                color_map[pix] = i

# Scan the image and write output
f = args.output

f.write(
    '{static} const {type} {name}[{width} * {height}] = {{\n'.format(
        static=('static' if args.static else ''),
        type=('u16' if args.bpp == 16 else 'u8'),
        name=args.name,
        width=img.width,
        height=img.height
    )
)

## 16 bpp
if args.bpp == 16:
    for y in range(img.height):
        for x in range(img.width):
            pix = img.getpixel( (x, y) )

            # convert pixel to a 15-bit color
            r = pix[0] >> 3
            g = pix[1] >> 3
            b = pix[2] >> 3

            col = (b << 10) | (g << 5) | r

            f.write('0x' + hex(col)[2:].zfill(4) + ',')
            if x % 8 == 7:
                f.write('\n')
        f.write('\n')

## 8 bpp
elif args.bpp == 8:
    for y in range(img.height):
        for x in range(img.width):
            pix = img.getpixel( (x, y) )

            if pix not in color_map:
                col = pix[0] << 16 | pix[1] << 8 | pix[2]
                col = hex(col)[2:].zfill(6)
                exit('Error: color not present in the palette: #' + col)

            f.write('0x' + hex(color_map[pix])[2:].zfill(2) + ',')
            if x % 8 == 7:
                f.write('\n')
        f.write('\n')

## 4 bpp
elif args.bpp == 4:
    pass # TODO

f.write('};\n')
