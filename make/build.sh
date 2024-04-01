#!/bin/bash

help()
{
    echo "build TCP_Srv_Echo sample"
    echo "options:"
    echo "-s  config; silent optimized for spped (default)"
    echo "-v  config: verbose with debug information"
    echo "-k  keep untracked artifacts"
    echo "-h  this help"
    exit
}

keep=
conf=silent
while getopts hkvs option; do
    case $option in
        (h)  help;;
        (k)  keep=1;;
        (v)  conf=verbose;;
        (s)  conf=silent;;
    esac
done

cd $(dirname $0)

git clean -dxf bin 2>/dev/null >/dev/null

if test -z $keep; then git clean -dxf . 2>/dev/null >/dev/null; fi

make -j -f TCP_Srv_Echo.make config=$conf

for b in  bin/*; do echo "-> $b"; done
