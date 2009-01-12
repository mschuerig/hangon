#! /bin/sh

set -e

gnulib-tool --import --m4-base=gnulib/m4 --source-base=gnulib/lib \
            --no-vc-files closeout getopt gettext

autoreconf --verbose --force --install
