#!/bin/bash

help()
{
    echo "Usage: $(basename $0) options [port]"
    echo "options:"
    echo "-p  config; prod"
    echo "-v  config: verbose"
    echo "-s  config: vsmall"
    echo ""
    echo "-r  run binary"
    echo "-h  this help"
    exit
}

conf=
run=
port=
while getopts hkrspv option; do
    case $option in
        (v)  conf=verbose;;
        (p)  conf=prod;;
        (s)  conf=vsmall;;
        (r)  run=1;;
        (h)  help;;
    esac
done

shift $(($OPTIND - 1))

if test -z $conf; then help; fi

cd $(dirname $0)

make config=$conf clean
make -j config=$conf

if test $? -ne 0; then exit 1; fi

bin=bin/TCP_Srv_Echo

if test -z $run; then
    echo "-> $bin"
else
    $bin $*
fi

