#!/bin/bash

help()
{
    echo "build TCP_Srv_Echo sample"
    echo "options:"
    echo "-S  config; silent"
    echo "-V  config: verbose"
    echo "-v  config: vsmall"
    echo "-k  keep untracked artifacts"
    echo "-r  run binary"
    echo "-h  this help"
    exit
}

keep=
conf=silent
run=
while getopts hkVSvr option; do
    case $option in
        (V)  conf=verbose;;
        (S)  conf=silent;;
        (v)  conf=vsmall;;
        (k)  keep=1;;
        (r)  run=1;;
        (h)  help;;
    esac
done

cd $(dirname $0)

git clean -dxf bin 2>/dev/null >/dev/null

if test -z $keep; then git clean -dxf . 2>/dev/null >/dev/null; fi

make -j -f TCP_Srv_Echo.make config=$conf

if test $? -ne 0; then exit 1; fi

bin=bin/TCP_Srv_Echo

if test ! -z $run; then
    $bin
else
    echo "-> $bin"
fi

