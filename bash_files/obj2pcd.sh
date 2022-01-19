#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/nyu_v2_filter/comparison/
objdir=$basedir'own/predv40/predictions_n00/'

if [[ ! -z "$1" ]] 
then 
    objdir=$1
fi
cd $objdir

for filename in *.obj; do
    cd $builddir
    # path
    ./obj2pcd $objdir$filename
done