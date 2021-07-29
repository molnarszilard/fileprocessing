#!bin/bash
builddir=$PWD'/../build/'
# basedir=/media/rambo/ssd2/Szilard/c24/
# basedir=/media/rambo/ssd2/Szilard/pico_tofnest/
# basedir=/media/rambo/ssd2/Szilard/nyu_tofnest/
basedir=/media/rambo/ssd2/Szilard/nyu_v2_filter/comparison/
# basedir=/media/rambo/ssd2/Szilard/toffilter_isaac/
# basedir=/media/rambo/ssd2/Szilard/kitti/validation/
ddir=$basedir'predictions/'
pcddir=$basedir'pcdpred/'
camera="nyu" #pico, nyu, kitti, isaac

if [[ ! -z "$1" ]] 
then 
    ddir=$1
    if [[ ! -z "$2" ]] 
    then 
        pcddir=$2
    fi
fi
cd $ddir

for filename in *.png; do
    cd $builddir
    # path
    ./depth2pcd $ddir $pcddir $filename $camera
done