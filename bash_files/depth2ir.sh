#!bin/bash
builddir=$PWD'/../build/'
basedir=/mnt/ssd1/datasets/c24/evaluation/
ddir=$basedir'noise/depth_n10/'
idir=$basedir'ir/'
outdir=$basedir'noise/depth2ir_n10/'

if [[ ! -z "$1" ]] 
then 
    ddir=$1
    if [[ ! -z "$2" ]] 
    then 
        idir=$2
        if [[ ! -z "$3" ]] 
        then 
            outdir=$3
        fi
    fi
fi
cd $ddir

declare -a arrD
for file in *0.png
do
    arrD=("${arrD[@]}" "$file")
done

cd $idir
declare -a arrI
for file in *0.png
do
    arrI=("${arrI[@]}" "$file")
done

cd $builddir

for i in "${!arrD[@]}"; do
    dfilename="${arrD[i]}"
    ifilename="${arrI[i]}"
    echo $ddir$dfilename
    # echo $ndir$nfilename
    ./depth2ir $ddir $idir $outdir $dfilename $ifilename
done