#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
#
# Version strings meson_version.py composes. A tagged release must come out as
# a bare X.Y.Z: anything appended makes the tarball name and the tag disagree.

import importlib.util
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
SCRIPT = os.path.join(HERE, os.pardir, "meson_version.py")

spec = importlib.util.spec_from_file_location("meson_version", SCRIPT)
meson_version = importlib.util.module_from_spec(spec)
spec.loader.exec_module(meson_version)

HASH = "0123456789ab"

CASES = [
    # (count, dirty, expected) for base 1.2.3
    ("0", False, "1.2.3"),                       # the tag itself
    ("0", True, "1.2.3+dirty"),
    ("7", False, "1.2.3+7.g" + HASH),            # past the tag
    ("7", True, "1.2.3+7.g" + HASH + ".dirty"),
    (None, False, "1.2.3+g" + HASH),             # archive: count unknown
    (None, True, "1.2.3+g" + HASH + ".dirty"),
]


def main():
    failures = 0
    for count, dirty, expected in CASES:
        got = meson_version.compose("1.2.3", count, HASH, dirty)
        status = "ok" if got == expected else "FAIL"
        if got != expected:
            failures += 1
            print("count=%r dirty=%r: expected %s, got %s" % (count, dirty, expected, got),
                  file=sys.stderr)
        print("  %-4s count=%-4r dirty=%-5r -> %s" % (status, count, dirty, got))

    # A release tag must never carry metadata, whatever else is true.
    if "+" in meson_version.compose("0.1.0", "0", HASH, False):
        print("a tagged release gained build metadata", file=sys.stderr)
        failures += 1

    sys.exit(1 if failures else 0)


if __name__ == "__main__":
    main()
