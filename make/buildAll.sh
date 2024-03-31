#!/bin/bash

help()
{
    echo "===================================="
    echo "build all major makes"
    echo "options:"
    echo "-r  remove all untracked artifacts"
    echo "-p  premake5 makefiles"
    echo "-h  this help"
    echo "===================================="
    exit
}

tm()
{
    en=$(date +%s)
    printf "%-25s[%3d]\n" $1 $(($en-$2))
}

mk()
{
    st=$(date +%s)
    make -j -f $1 > /dev/null
    tm ${1%.*} $st
}

rem=
exe=
pre=
while getopts hrp option; do
    case $option in
      (h)  help;;
      (r)  rem=1;;
      (p)  pre=1
    esac
done

cd $(dirname $0)

if test ! -z $rem; then git clean -dxf . 2>/dev/null >/dev/null; pre=1; fi

if test ! -z $pre; then premake5 gmake2 > /dev/null; fi

st=$(date +%s)

for fn in *.make; do mk $fn; done
wait
echo binaries:
for b in  bin/*; do echo "- $b"; done
