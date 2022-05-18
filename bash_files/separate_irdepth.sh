#!bin/bash
builddir=$PWD'/../build/'
basedir=/mnt/ssd1/datasets/c24/
ddir=$basedir'adepth/'
idir=$basedir'air/'
indir=$basedir'adepth2ir/test/'

if [[ ! -z "$1" ]] 
then 
    ddir=$1
    if [[ ! -z "$2" ]] 
    then 
        idir=$2
        if [[ ! -z "$3" ]] 
        then 
            indir=$3
        fi
    fi
fi
cd $indir

declare -a arrIN
for file in *.png
do
    arrIN=("${arrIN[@]}" "$file")
done

cd $builddir

for i in "${!arrIN[@]}"; do
    infilename="${arrIN[i]}"
    echo $indir$infilename
    ./separate_irdepth $ddir $idir $indir $infilename
done