#!/bin/python

from sys import argv

with open(argv[1], 'rb') as f:
    f.seek(0xa0)
    checksum = -(0x19 + sum(f.read(29))) & 0xff

print(hex(checksum))
