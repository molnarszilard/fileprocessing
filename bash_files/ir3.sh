#!bin/bash
builddir=$PWD'/../build/'
basedir=/mnt/ssd1/datasets/c24/evaluation/
idir=$basedir'ir/'
ir3dir=$basedir'ir3/'


if [[ ! -z "$1" ]] 
then 
    idir=$1
    if [[ ! -z "$2" ]] 
    then 
        ir3dir=$2
    fi
fi
cd $idir

for filename in *.png; do
    cd $builddir
    echo $idir$filename
    # depth directory, mask directory, filename
    ./ir3 $idir $ir3dir $filename
done