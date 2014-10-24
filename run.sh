#!/bin/sh
#$ -S /bin/sh

die () {
    echo >&2 "$@"
    exit 1
}

[ "$#" -eq 1 ] || die "1 argument required"

i=$1

cd /home/usuaris/gilabert/spliter

./countdown $i
