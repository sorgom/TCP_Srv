#!/bin/bash

help()
{
    echo "Usage: $(basename $0) options [port] [locale]"
    echo "options:"
    echo "configurations (select one):"
    echo "  -p  config; prod"
    echo "  -v  config: verbose"
    echo "  -s  config: vsmall"
    echo "behaviour:"
    echo "  -c  clean all untracked artifacts"
    echo "  -r  run binary with [port] [locale]"
    echo "  -h  this help"
    exit
}

conf=
run=
port=
clean=
while getopts pvscrh option; do
    case $option in
        (p)  conf=prod;;
        (v)  conf=verbose;;
        (s)  conf=vsmall;;
        (c)  clean=1;;
        (r)  run=1;;
        (h)  help;;
    esac
done

shift $(($OPTIND - 1))

if test -z $conf; then help; fi

cd $(dirname $0)

if test -v $clean; then git clean -dfx . 2>/dev/null >/dev/null; fi

make config=$conf clean >/dev/null
make -j config=$conf

if test $? -ne 0; then exit $?; fi

bin=bin/EchoSrv_$conf

if test ! -f $bin; then exit 1; fi

if test -z $run; then
    echo "-> $bin"
else
    echo ""
    echo "starting $bin $*"
    echo ""
    $bin $*
fi

