#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/nyu_v2_filter/comparison/
objdir=$basedir'own/predv40/predictions_n00/'
sampling=0


if [[ ! -z "$1" ]] 
then 
    objdir=$1
    if [[ ! -z "$2" ]] 
    then 
        sampling=$2
    fi
fi
cd $objdir

for filename in *.obj; do
    cd $builddir
    # path
    ./obj2pcd $objdir$filename $sampling
done