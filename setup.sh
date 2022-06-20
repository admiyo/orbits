#!/bin/sh

autoreconf --install
./configure
automake
make
