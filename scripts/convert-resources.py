#!/bin/python

# Copyright 2023-2024 Vulcalien
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

# Setup argparse
parser = argparse.ArgumentParser(
    description='Convert all resources listed in JSON files'
)

parser.add_argument('res_list_files', nargs='+',
                    type=argparse.FileType('r'),
                    help='specify the resource list files (JSON files)')

args = parser.parse_args()

# Functions

def tileset_args(element):
    tile_width  = int(element['tile_width'])
    tile_height = int(element['tile_height'])

    palette = str(element['palette'])
    bpp     = int(element['bpp'])

    return "--tile-width %d --tile-height %d --palette %s --bpp %d" % (
        tile_width, tile_height, palette, bpp
    )

def image_args(element):
    bpp = int(element['bpp'])

    if bpp in (4, 8):
        palette = str(element['palette'])
    else:
        palette = None

    return "--bpp %d %s" % (
        bpp, (('--palette %s' % palette) if palette else '')
    )

FILE_TYPES = {
    'tilesets': {
        'script': '%s/res/tileset-to-array.py',
        'args_function': tileset_args
    },

    'palettes': {
        'script': '%s/res/palette-to-array.py',
        'args_function': None
    },

    'images': {
        'script': '%s/res/image-to-array.py',
        'args_function': image_args
    },

    'files': {
        'script': '%s/res/file-to-array.py',
        'args_function': None
    }
}

PARENT_PATH = os.path.relpath(sys.path[0])

def convert(element, file_type):
    input_file  = str(element['input'])
    output_file = str(element['output'])
    name        = str(element['name'])

    static = False
    if 'static' in element and bool(element['static']):
        static = True

    cmd = ' '.join((
        file_type['script'] % PARENT_PATH,
        '-i', input_file,
        '-o', output_file,
        '-n', name,
        '-s' if static else ''
    ))

    # Add script-specific arguments
    args_function = file_type['args_function']
    if args_function is not None:
        cmd += ' ' + args_function(element)

    print(cmd)
    os.system(cmd)

# Read resource list files
for f in args.res_list_files:
    try:
        content = json.load(f)
    except json.JSONDecodeError as e:
        print('Error: invalid JSON:', e)
        continue

    for file_type in FILE_TYPES:
        if file_type in content:
            for element in content[file_type]:
                convert(element, FILE_TYPES[file_type])
