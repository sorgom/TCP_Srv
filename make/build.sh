#!/bin/bash

help()
{
    echo "Usage: $(basename $0) options"
    echo "options:"
    echo "-p  config; prod"
    echo "-v  config: verbose"
    echo "-s  config: vsmall"
    echo ""
    echo "-k  keep untracked artifacts"
    echo "-r  run binary"
    echo "-h  this help"
    exit
}

keep=
conf=
run=
while getopts hkrspv option; do
    case $option in
        (v)  conf=verbose;;
        (p)  conf=prod;;
        (s)  conf=vsmall;;
        (k)  keep=1;;
        (r)  run=1;;
        (h)  help;;
    esac
done

if test -z $conf; then help; fi

cd $(dirname $0)

if test -z $keep; then git clean -dxf . 2>/dev/null >/dev/null; fi

make config=$conf clean
make -j config=$conf

if test $? -ne 0; then exit 1; fi

bin=bin/TCP_Srv_Echo

if test ! -z $run; then
    $bin
else
    echo "-> $bin"
fi

