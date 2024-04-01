#!/bin/bash

help()
{
    echo "===================================="
    echo "build all major makes"
    echo "options:"
    echo "-r  remove all untracked artifacts"
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
while getopts hr option; do
    case $option in
      (h)  help;;
      (r)  rem=1;;
    esac
done

cd $(dirname $0)

if test ! -z $rem; then git clean -dxf . 2>/dev/null >/dev/null; fi

st=$(date +%s)

for fn in *.make; do mk $fn; done
wait
echo binaries:
for b in  bin/*; do echo "- $b"; done
