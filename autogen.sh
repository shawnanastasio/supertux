#!/bin/sh

# we need a minimum of automake 1.6 and automake 1.8 seems to be buggy
# this doesn't seem to work well
# see AUTOMAKE_OPTIONS in Makefile.am
export WANT_AUTOMAKE=1.6

autoheader
libtoolize --force
aclocal -I m4
automake --copy --add-missing
autoconf

