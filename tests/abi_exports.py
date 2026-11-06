#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
#
# The shared library must export exactly the functions the public headers
# mark SCLI_API: no internal symbol leaking out, no public one missing.
#
#   abi_exports.py <nm> <libsparkcli.so> <header>...

import re
import subprocess
import sys

# SCLI_API <return type> name(
DECL = re.compile(r"\bSCLI_API\b[^;(]*?\b([A-Za-z_][A-Za-z0-9_]*)\s*\(")


def declared(headers):
    names = set()
    for path in headers:
        with open(path, encoding="utf-8") as handle:
            text = handle.read()
        # Drop comments so a commented-out declaration does not count, and
        # preprocessor lines so api.h's own #define of SCLI_API is not read
        # as a declaration.
        text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
        text = re.sub(r"//[^\n]*", "", text)
        text = re.sub(r"^[ \t]*#[^\n]*", "", text, flags=re.M)
        names.update(DECL.findall(text))
    return names


def exported(nm, library):
    out = subprocess.run([nm, "--dynamic", "--defined-only", library],
                         capture_output=True, text=True, check=True).stdout
    names = set()
    for line in out.splitlines():
        parts = line.split()
        # "<addr> T <name>"; T is an exported text symbol.
        if len(parts) == 3 and parts[1] == "T":
            names.add(parts[2])
    return names


def main(argv):
    if len(argv) < 3:
        sys.exit("usage: abi_exports.py <nm> <library> <header>...")
    nm, library, headers = argv[0], argv[1], argv[2:]

    want = declared(headers)
    have = exported(nm, library)
    if not want:
        sys.exit("abi_exports.py: no SCLI_API declarations found in %s" % " ".join(headers))

    leaked = sorted(have - want)
    missing = sorted(want - have)

    for name in leaked:
        print("exported but not declared SCLI_API: %s" % name, file=sys.stderr)
    for name in missing:
        print("declared SCLI_API but not exported: %s" % name, file=sys.stderr)
    if leaked or missing:
        sys.exit(1)

    print("%d exported symbols match the public headers" % len(want))


if __name__ == "__main__":
    main(sys.argv[1:])
