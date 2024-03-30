#!/bin/bash

help()
{
    echo "===================================="
    echo "build all major makes"
    echo "options:"
    echo "-r  remove all artifacts before"
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
while getopts hrxp option; do
    case $option in
      (h)  help;;
      (r)  rem=1;;
      (x)  exe=1;;
      (p)  pre=1
    esac
done

cd $(dirname $0)

if test ! -z $rem; then git clean -dxf . 2>/dev/null >/dev/null; pre=1; fi

if test ! -z $pre; then premake5 gmake2 > /dev/null; fi

st=$(date +%s)
rm -f $(ls *.make | grep '_MS_') $(ls *.make | grep '_MS_')

for fn in $(ls *.make | grep '^[0-9][0-9]'); do mk $fn; done
wait
echo binaries:
for b in  bin/*; do echo "- $b"; done
tm "DONE" $st
