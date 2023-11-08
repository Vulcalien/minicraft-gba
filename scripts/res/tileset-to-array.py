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
    description='Generate a GBA tileset from a tileset image'
)

parser.add_argument('-i', '--input',
                    type=argparse.FileType('rb'), required=True,
                    help='specify the filename of the image file')
parser.add_argument('-n', '--name',
                    required=True,
                    help='specify the name of the output array')
parser.add_argument('--bpp',
                    type=int, choices=[4, 8], required=True,
                    help='specify the bits per pixel')
parser.add_argument('--tile-width',
                    type=int, required=True,
                    help='specify the width of a tile')
parser.add_argument('--tile-height',
                    type=int, required=True,
                    help='specify the height of a tile')

parser.add_argument('--palette',
                    type=argparse.FileType('rb'), required=True,
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

tileset_w = (img.width  // 8) // args.tile_width
tileset_h = (img.height // 8) // args.tile_height

# Map the colors by reading the palette image
color_map = {}

palette_img = Image.open(args.palette).convert('RGB')
for y in range(palette_img.height):
    for x in range(palette_img.width):
        pix = palette_img.getpixel( (x, y) )

        if pix not in color_map:
            i = (x + y * palette_img.width) % (2 ** args.bpp)
            color_map[pix] = i

# Scan the tileset and write output
f = args.output

f.write(
    '{static} const u8 {name}[{a} * {b} * {c}] = {{\n'.format(
        static=('static' if args.static else ''),
        name=args.name,
        a=(tileset_w * tileset_h),
        b=(args.tile_width * args.tile_height),
        c=(32 if args.bpp == 4 else 64)
    )
)

### helper functions ###
def scan_8pixel_line(x0, y):
    # 8bpp
    if args.bpp == 8:
        for xpix in range(8):
            pix = img.getpixel( (x0 + xpix, y) )

            if pix not in color_map:
                col = pix[0] << 16 | pix[1] << 8 | pix[2]
                col = hex(col)[2:].zfill(6)
                exit('Error: color not present in the palette: #' + col)

            f.write('0x' + hex(color_map[pix])[2:].zfill(2) + ',')

    # 4 bpp
    elif args.bpp == 4:
        for xpix in (1, 0, 3, 2, 5, 4, 7, 6):
            pix = img.getpixel( (x0 + xpix, y) )

            if pix not in color_map:
                col = pix[0] << 16 | pix[1] << 8 | pix[2]
                col = hex(col)[2:].zfill(6)
                exit('Error: color not present in the palette: #' + col)

            if xpix & 1 == 1:
                f.write('0x')
            f.write(hex(color_map[pix])[2:])
            if xpix & 1 == 0:
                f.write(',')
### ---------------- ###

for yt in range(tileset_h):
    for xt in range(tileset_w):
        for ysubtile in range(args.tile_height):
            for xsubtile in range(args.tile_width):
                for ypix in range(8):
                    scan_8pixel_line(
                        (xt * args.tile_width  + xsubtile) * 8,
                        (yt * args.tile_height + ysubtile) * 8 + ypix
                    )

                    f.write('\n')
                f.write('\n')

f.write('};\n')
