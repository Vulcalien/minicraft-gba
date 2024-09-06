#!/bin/python

from sys import argv

with open(argv[1], 'rb') as f:
    checksum = 0
    for i in range(128 * 1024 - 4):
        checksum += int.from_bytes(f.read(1))
    checksum &= 0xffffffff

    checksum_in_file = int.from_bytes(f.read(4), 'little')

print('Calculated:   ' + hex(checksum))
print('In save file: ' + hex(checksum_in_file))
print('Result:       ' + str(checksum_in_file == checksum))
