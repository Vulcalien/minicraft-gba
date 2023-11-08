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

# Resource List File example:
""" resources.json
{
    "tilesets": [
        {
            "name": "my_tileset",

            "input": "my/input/tileset.png",
            "output": "my/output/tileset.c",

            "tile_width": 2,
            "tile_height": 4,

            "palette": "my/input/palette.png",
            "bpp": 4,

            "static": true
        },

        ...
    ],

    "palettes": [
        {
            "name": "my_palette",

            "input": "my/input/palette.png",
            "output": "my/output/palette.c",

            "static": false
        },

        ...
    ],

    "images": [
        {
            "name": "image_8bpp",

            "input": "my/input/8bpp_image.png",
            "output": "my/output/8bpp_image.c",

            "palette": "my/input/palette.png",
            "bpp": 8,

            "static": false
        },

        {
            "name": "image_16bpp",

            "input": "my/input/16bpp_image.png",
            "output": "my/output/16bpp_image.c",

            "bpp": 16,

            "static": false
        },

        ...
    ],

    "files": [
        {
            "name": "my_file",

            "input": "my/input/file.bin",
            "output": "my/output/file.c",

            "static": true
        },

        ...
    ]
}
"""
# Optional values:
#   - 'static', if absent, defaults to False
#   - in 'images', 'palette' is ignored and not required if 'bpp' is 16

import sys, os, argparse, json

parent_path = os.path.relpath(sys.path[0])

# Setup argparse
parser = argparse.ArgumentParser(
    description='Convert all resources listed in JSON files'
)

parser.add_argument('res_list_files', nargs='+',
                    type=argparse.FileType('r'),
                    help='specify the resource list files (JSON files)')

args = parser.parse_args()

# Functions
def basic_info(element):
    return (
        str(element['input']), str(element['output']),
        str(element['name']),
        (bool(element['static']) if 'static' in element else False)
    )

def convert_tileset(element):
    (input_file, output_file, name, static) = basic_info(element)

    tile_width  = int(element['tile_width'])
    tile_height = int(element['tile_height'])

    palette = str(element['palette'])
    bpp     = int(element['bpp'])

    cmd = "%s/res/tileset-to-array.py -i %s -o %s -n %s %s " +\
          "--tile-width %d --tile-height %d " +\
          "--palette %s --bpp %d"
    cmd %= (
        parent_path, input_file, output_file, name,
        ('-s' if static else ''), tile_width, tile_height,
        palette, bpp
    )

    print(cmd)
    os.system(cmd)

def convert_palette(element):
    (input_file, output_file, name, static) = basic_info(element)

    cmd = "%s/res/palette-to-array.py -i %s -o %s -n %s %s"
    cmd %= (
        parent_path, input_file, output_file, name,
        ('-s' if static else '')
    )

    print(cmd)
    os.system(cmd)

def convert_image(element):
    (input_file, output_file, name, static) = basic_info(element)

    bpp = int(element['bpp'])

    if bpp in (4, 8):
        palette = str(element['palette'])
    else:
        palette = None

    cmd = "%s/res/image-to-array.py -i %s -o %s -n %s %s" +\
          "--bpp %d %s"
    cmd %= (
        parent_path, input_file, output_file, name,
        ('-s' if static else ''), bpp,
        (('--palette %s' % palette) if palette else '')
    )

    print(cmd)
    os.system(cmd)

def convert_file(element):
    (input_file, output_file, name, static) = basic_info(element)

    cmd = "%s/res/file-to-array.py -i %s -o %s -n %s %s"
    cmd %= (
        parent_path, input_file, output_file, name,
        ('-s' if static else '')
    )

    print(cmd)
    os.system(cmd)

# Read resource list files
for f in args.res_list_files:
    try:
        content = json.load(f)
    except json.JSONDecodeError as e:
        print('Error: invalid JSON:', e)
        continue

    if 'tilesets' in content:
        for element in content['tilesets']:
            convert_tileset(element)

    if 'palettes' in content:
        for element in content['palettes']:
            convert_palette(element)

    if 'images' in content:
        for element in content['images']:
            convert_image(element)

    if 'files' in content:
        for element in content['files']:
            convert_file(element)
