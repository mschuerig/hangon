#! /bin/sh

set -e

autoreconf --verbose --force --install
gettextize --force
