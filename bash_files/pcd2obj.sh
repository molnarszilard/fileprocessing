#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/nyu_v2_filter/comparison/
pcddir=$basedir'own/predv40/predictions_n00/'

if [[ ! -z "$1" ]] 
then 
    pcddir=$1
fi
cd $pcddir

for filename in *.obj; do
    cd $builddir
    # path
    ./pcd2obj $pcddir$filename
done