#!/bin/bash

configs="prod verbose vsmall"

help()
{
    echo "Usage: $(basename $0) options [port] [locale]"
    echo "options:"
    echo "-c  clean ignored artifacts before"
    echo "-r  <config> run binary with [port] [locale]"
    echo "    with <config> in $configs"
    echo "-p  premake5 makefiles"
    echo "-h  this help"
    exit
}

run=
clean=
pre=
while getopts cr:ph option; do
    case $option in
        (c)  clean=1;;
        (r)  run=$OPTARG;;
        (p)  pre=1;;
        (h)  help;;
    esac
done

shift $(($OPTIND - 1))

function tm()
{
    en=$(date +%s)
    printf "%-10s[%3d]\n" $1 $(($en-$2))
}

function mkconfig
{
    st=$(date +%s)
    make -j config=$1 >/dev/null
    if test $? -ne 0; then return 1; fi
    tm ${1%.*} $st
    return 0
}

cd $(dirname $0)

if test ! -z $pre; then premake5 gmake2; fi

if test ! -z $clean; then git clean -dfXq . ; fi
echo building congigurations ...

pids=()
for config in $configs; do
    mkconfig $config & pids+=($!)
done

ecode=0
for pid in ${pids[*]}; do
    if ! wait $pid; then ecode=1; fi
done

if test -z $run; then
    builds=$(ls bin/* 2>/dev/null)
    if test ! -z "$builds"; then
        echo built:
        for b in $builds; do
            echo - $b
        done
    fi
    exit $ecode;
fi

bin=$(ls bin/*_$run 2>/dev/null | head -n 1)
if test -z $bin; then
    echo "no binary for config $run"
    exit 1
fi

echo ""
echo "starting $bin $*"
echo ""
$bin $*
