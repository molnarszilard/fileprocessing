#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/c24/
# basedir=/media/rambo/ssd2/Szilard/pico_tofnest/
# basedir=/media/rambo/ssd2/Szilard/nyu_tofnest/
ddir=$basedir'depth/'
normaldir=$basedir'normalimages/'
camera="isaac" #pico, nyu, kitti, isaac
supportsize=0.05

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
    ./compute_normals_image $ddir $normaldir $filename $camera $supportsize
done