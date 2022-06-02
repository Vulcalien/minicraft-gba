#!/bin/python

while True:
    col = int(input("input: "), 16)

    r = (col >> 16) & 0xff
    g = (col >> 8) & 0xff
    b = col & 0xff

    col15 = ((b >> 3) << 10) | ((g >> 3) << 5) | (r >> 3)
    print('0x' + hex(col15)[2:].zfill(4))
