#!/usr/bin/env python3

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
from PIL import Image

# Setup argparse
parser = argparse.ArgumentParser(
    description='Generate a GBA palette from an image'
)

parser.add_argument('-i', '--input',
                    type=argparse.FileType('rb'), required=True,
                    help='specify the filename of the image file')
parser.add_argument('-n', '--name',
                    required=True,
                    help='specify the name of the output array')

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

palette_size = img.width * img.height

# Get and convert colors
colors = []
for i in range(palette_size):
    pix = img.getpixel( (i % img.width, i // img.width) )

    r = pix[0] >> 3
    g = pix[1] >> 3
    b = pix[2] >> 3

    col = (b << 10) | (g << 5) | r

    color_code = '0x' + hex(col)[2:].zfill(4)
    colors.append(color_code)

# Write output
f = args.output

f.write(
    '{static} const u16 {name}[{a} * 16 + {b}] = {{\n'.format(
        static=('static' if args.static else ''),
        name=args.name,
        a=(palette_size // 16),
        b=(palette_size %  16)
    )
)

for i in range(palette_size):
    f.write(colors[i] + ',')
    if i % 8 == 7:
        f.write('\n')
    if i % 16 == 15 or i == palette_size - 1:
        f.write('\n')

f.write('};\n')
