#!/usr/bin/env python3

import argparse
import subprocess
from elftools.elf.elffile import ELFFile
from icecream import ic
import re

def generate_hashes(filepath: str) -> dict:
    from elftools.elf.sections import SymbolTableSection, Symbol

    try:
        elf = ELFFile(open(filepath, "r+b"))
    except Exception as e:
        print(f"Couldn't open input file {filepath}: {e}")
        exit(1)

    # Grab the format table
    symtab = elf.get_section_by_name(".symtab")

    # Get the symbols from the symbol table
    fmt, id = [], []
    for s in symtab.iter_symbols():
        if "defmt_id" in s.name:
            id.append(s)
        elif "defmt_fmt" in s.name:
            fmt.append(s)
    
    id = sorted(id, key=lambda x: x.name)
    fmt = sorted(fmt, key=lambda x: x.name)
    assert len(id) == len(fmt)

    import hashlib

    id_sect = elf.get_section_by_name("defmt_id")
    fmt_sect = elf.get_section_by_name("defmt_fmt")
    num_regex = re.compile("defmt_id([0-9]*)")
    out_table = []
    assert id_sect and fmt_sect
    for id, fmt in zip(id, fmt):
        num_id = num_regex.findall(id.name)[0]
        nbytes = id.entry.st_size

        s = fmt.entry.st_value
        e = s + fmt.entry.st_size
        fmt_strn = fmt_sect.data()[s:e]
        fmt_hash = hashlib.sha256(fmt_strn).digest()[0:nbytes]
        fmt_hash = int.from_bytes(fmt_hash)

        out_table.append([fmt_hash, fmt_strn.decode('utf-8')])
        binoffset = id_sect.header.sh_offset + id.entry.st_value

        elf.stream.seek(binoffset)
        before = int.from_bytes(elf.stream.read(id.entry.st_size))

        elf.stream.seek(binoffset)
        elf.stream.write(fmt_hash.to_bytes(nbytes))

        elf.stream.seek(binoffset)
        after = int.from_bytes(elf.stream.read(id.entry.st_size))

        print(f"Id: {num_id}, fmt: {fmt_strn.decode('utf-8')}")
        print(f"    {hex( before )} -> {hex( after )}")

    out_table = sorted(out_table, key = lambda x: x[0])
    return out_table


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="Build a binary patch from the given ELF files"
    )
    parser.add_argument("file", help="Path to the base image file")
    parser.add_argument("--out", default="dict.txt")
    args = parser.parse_args()

    map = generate_hashes(args.file)
    with(open(args.out, 'w') as f):
        dict = "\n".join([f"{id}: {fmt}" for id,fmt in map])
        f.write(dict)
