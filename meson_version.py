#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
#
# Project version from git, or from the version data of a source archive.
#
#   meson_version.py                   print the version (project() in meson.build)
#   meson_version.py --parts           print the version split into key=value parts
#   meson_version.py --dist <version>  write <version> into the `meson dist` tree
#
# Sources, first match wins:
#  1. git, when the source dir is the top of its own work tree (not an
#     archive unpacked inside some other repository).
#  2. .tarball-version, written by --dist into `meson dist` tarballs.
#  3. .archive-version, filled in by `git archive` through export-subst
#     (also GitHub "Source code" downloads).
# Anything else is an error, never a guess.
#
# Versions ("+" part is SemVer build metadata):
#   HEAD tagged vX.Y.Z         -> X.Y.Z
#   N commits after vX.Y.Z     -> X.Y.Z+N.g<hash>
#   no vX.Y.Z tag reachable    -> 0.0.0+N.g<hash>
#   no commit yet              -> 0.0.0+unborn
#   uncommitted changes        -> <any of the above> with ".dirty" metadata
#
# Written in Python rather than shell because SparkCLI is also built on
# Windows, where the MSVC job has no POSIX shell.

import os
import re
import subprocess
import sys

SRC = os.path.dirname(os.path.abspath(__file__))
TARBALL_FILE = ".tarball-version"
ARCHIVE_FILE = ".archive-version"


def git(*args):
    """Run git in the source tree, returning stripped stdout or None."""
    try:
        out = subprocess.run(
            ["git", "-C", SRC, *args],
            capture_output=True, text=True, check=True,
        )
    except (OSError, subprocess.CalledProcessError):
        return None
    return out.stdout.strip()


def compose(base, count, commit_hash, dirty):
    """Join the release with its build metadata, SemVer style.

    count is "0" when HEAD is the tag itself, "N" for N commits past it, and
    None when it is not available (a git archive records no commit count).
    A tagged release carries no build metadata: it is just X.Y.Z.
    """
    meta = []
    if count != "0":
        meta.append("%s.g%s" % (count, commit_hash) if count else "g%s" % commit_hash)
    if dirty:
        meta.append("dirty")
    return base + ("+" + ".".join(meta) if meta else "")


def git_version(allow_dirty):
    """Version from git, or None when this is not our own work tree."""
    # An archive unpacked inside an unrelated repository must not borrow its
    # version, so only the top of a work tree counts.
    prefix = git("rev-parse", "--show-prefix")
    if prefix is None or prefix != "":
        return None
    if git("rev-parse", "-q", "--verify", "HEAD") is None:
        return "0.0.0+unborn"

    commit_hash = git("rev-parse", "--short=12", "HEAD")
    if commit_hash is None:
        return None

    described = git("describe", "--tags", "--long", "--match", "v[0-9]*", "HEAD")
    if described:
        base_and_count = described.rsplit("-g", 1)[0]   # vX.Y.Z-N
        base, _, count = base_and_count.rpartition("-")
        base = base.lstrip("v")
    else:
        count = git("rev-list", "--count", "HEAD")
        base = "0.0.0"
        if count is None:
            return None

    dirty = False
    if allow_dirty:
        status = git("--no-optional-locks", "status", "--porcelain", "--untracked-files=no")
        dirty = bool(status)

    return compose(base, count, commit_hash, dirty)


def archive_version():
    """Version from the placeholders git archive expands, or None."""
    path = os.path.join(SRC, ARCHIVE_FILE)
    if not os.path.isfile(path):
        return None
    with open(path, encoding="utf-8") as handle:
        text = handle.read()

    commit = re.search(r"^commit: (.*)$", text, re.M)
    described = re.search(r"^describe: (.*)$", text, re.M)
    commit = commit.group(1).strip() if commit else ""
    described = described.group(1).strip() if described else ""

    # Unexpanded placeholders mean a plain copy of a checkout, not an archive.
    if not re.fullmatch(r"[0-9a-f]+", commit):
        return None
    commit_hash = commit[:12]

    if not described:
        return compose("0.0.0", None, commit_hash, False)
    tagged = re.fullmatch(r"(v[0-9][^-]*)-([0-9]+)-g[0-9a-f]+", described)
    if tagged:
        return compose(tagged.group(1).lstrip("v"), tagged.group(2), commit_hash, False)
    if re.fullmatch(r"v[0-9].*", described):
        return compose(described.lstrip("v"), "0", commit_hash, False)
    return None


def parse_version(version):
    """Split a composed version into its parts.

    X.Y.Z, optionally followed by "+" and metadata: the commit count, the
    g-prefixed hash and "dirty", any of which may be absent.
    """
    core, _, meta = version.partition("+")
    numbers = core.split(".")
    if len(numbers) != 3 or not all(n.isdigit() for n in numbers):
        sys.exit("meson_version.py: version does not start with X.Y.Z: %s" % version)

    parts = {
        "major": numbers[0],
        "minor": numbers[1],
        "patch": numbers[2],
        "commits": "0",
        "commit": "",
        "dirty": "0",
    }
    for item in meta.split(".") if meta else []:
        if item == "dirty":
            parts["dirty"] = "1"
        elif item.isdigit():
            parts["commits"] = item
        elif item.startswith("g"):
            parts["commit"] = item[1:]
    return parts


def read_version():
    version = git_version(allow_dirty=True)
    if version:
        return version
    tarball = os.path.join(SRC, TARBALL_FILE)
    if os.path.isfile(tarball):
        with open(tarball, encoding="utf-8") as handle:
            return handle.read().strip()
    version = archive_version()
    if version:
        return version
    sys.exit(
        "meson_version.py: no version for %s: not a git checkout, a "
        "'meson dist' tarball (%s) or a git archive (%s)" % (SRC, TARBALL_FILE, ARCHIVE_FILE)
    )


def write_dist_version(configured):
    """Record the version inside the dist tree, refusing a stale build dir."""
    dist_root = os.environ.get("MESON_PROJECT_DIST_ROOT") or os.environ.get("MESON_DIST_ROOT")
    if not dist_root:
        sys.exit("meson_version.py: --dist is only run by 'meson dist'")
    version = git_version(allow_dirty=False)
    if not version:
        sys.exit("meson_version.py: --dist needs the git work tree at %s" % SRC)
    if version != configured:
        sys.exit(
            "meson_version.py: build dir configured as %s but HEAD is %s; "
            "commit or stash, then 'meson setup --reconfigure'" % (configured, version)
        )
    with open(os.path.join(dist_root, TARBALL_FILE), "w", encoding="utf-8") as handle:
        handle.write(version + "\n")


def main(argv):
    if not argv:
        print(read_version())
    elif argv[0] == "--parts":
        # key=value lines, for meson to turn into configuration data.
        version = read_version()
        parts = parse_version(version)
        parts["string"] = version
        for key in ("major", "minor", "patch", "commits", "commit", "dirty", "string"):
            print("%s=%s" % (key, parts[key]))
    elif argv[0] == "--dist":
        if len(argv) != 2:
            sys.exit("usage: meson_version.py --dist <configured version>")
        write_dist_version(argv[1])
    else:
        sys.exit("usage: meson_version.py [--parts | --dist <configured version>]")


if __name__ == "__main__":
    main(sys.argv[1:])
