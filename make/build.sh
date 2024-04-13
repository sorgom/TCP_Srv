#!/bin/bash
#   ============================================================
#   build and run script for gcc / make
#   ============================================================
#   created by Manfred Sorgo

#   extract all configurations from make help
getcfgs()
{
    take=0
    for item in $(make help); do
        if [[ $item =~ CONFIG.*: ]]; then take=1
        elif [[ $item =~ : ]]; then take=0    
        elif test $take -eq 1; then 
            cfgs="$cfgs $item"
            cfgh="$cfgh, $item"
        fi
    done
    cfgs=${cfgs:1}
    cfgh=${cfgh:2}
}

help()
{
    echo "Usage: $(basename $0) [options] [port] [locale]"
    echo "options:"
    echo "-c  clean ignored artifacts before"
    echo "-r  <config> run binary with [port] [locale]"
    echo "    with <config> in $cfgh"
    echo "-p  premake5 makefiles"
    echo "-h  this help"
    exit
}

cd $(dirname $0)
getcfgs

run=
clean=
pre=
while getopts cr:ph option; do
    case $option in
        (c)  clean=1;;
        (r)  run=$OPTARG;;
        (p)  pre=1;clean=1;;
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

if test ! -z $clean; then git clean -dfXq . ; fi

if test ! -z $pre; then premake5 gmake2; fi

echo building congigurations ...

#   start build all build tasks in background
#   save pids in array
pids=()
for config in $cfgs; do
    mkconfig $config & pids+=($!)
done

#   wait for all build tasks to finish
#   and evaluate return code
err=0
for pid in ${pids[*]}; do
    if ! wait $pid; then err=1; fi
done

if test $err -ne 0; then exit 1; fi

if test -z $run; then
    builds=$(ls bin/* 2>/dev/null)
    if test ! -z "$builds"; then
        echo built:
        for b in $builds; do
            echo - $b
        done
    fi
    exit 0;
fi

bin=$(ls bin/*_$run 2>/dev/null | head -n 1)
if test -z $bin; then
    echo "no binary for config '$run'"
    exit 1
fi

echo ""
echo "starting $bin $*"
echo ""
exec $bin $*
