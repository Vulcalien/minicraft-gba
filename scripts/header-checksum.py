#!/bin/python

from sys import argv

checksum = 0

with open(argv[1], 'rb') as f:
    f.seek(0xa0)
    checksum -= sum(f.read(29))

checksum -= 0x19
checksum &= 0xff

print(hex(checksum))
