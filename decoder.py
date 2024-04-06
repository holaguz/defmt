#!/usr/bin/env python3

import argparse
import re
import sys
import struct

def parse_dict(s: str):
    lines = s.split('\n')
    lines = [l for l in lines if l != '\x00']
    m = re.compile('([0-9]*): (.*)')
    d = {}
    for l in lines:
        id, fmt = m.findall(l)[0]
        id = int(id)
        d[id] = fmt

    return d


DELIM = b"\xDD\xEE\xFFM"

if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="Build a binary patch from the given ELF files"
    )
    # parser.add_argument("input", help="Path to the dictionary")
    parser.add_argument("dict", help="Path to the dictionary")
    args = parser.parse_args()

    with open(args.dict, "r") as f:
        map = f.read()

    d = parse_dict(map)

    buf = bytes()
    while True:
        while True:
            buf += sys.stdin.buffer.read(1)
            if len(buf) >= len(DELIM) and buf[-len(DELIM):] == DELIM:
                break
        
        id = int.from_bytes(buf[0:8])
        rest = buf[8:]
        fmt = d[id];
        print(f"id: {id}, fmt: {fmt}, payload: {rest}")
        buf = bytes()
