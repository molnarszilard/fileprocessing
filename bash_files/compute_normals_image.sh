#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/c24/640x480/
# basedir=/media/rambo/ssd2/Szilard/pico_tofnest/
# basedir=/media/rambo/ssd2/Szilard/nyu_tofnest/
ddir=$basedir'aug/depth/'
normaldir=$basedir'aug/rgb/'
camera="isaac" #pico, nyu, kitti, isaac
supportsize=1.0

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