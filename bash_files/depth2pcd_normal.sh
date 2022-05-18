#!bin/bash
builddir=$PWD'/../build/'
# basedir=/media/rambo/ssd2/Szilard/kitti/validation/
basedir=/mnt/ssd1/datasets/c24/evaluation/noise/
# basedir=/media/rambo/ssd2/Szilard/nyu_tofnest/
# basedir=/media/rambo/ssd2/Szilard/lenssen_tofnest/
ddir=$basedir'depth_n10/'
ndir=$basedir'normal_pred_ir2d_n10/'
pcddir=$basedir'pcdpred_ir2d_n10/'
camera="pico" #pico, nyu, kitti, isaac

if [[ ! -z "$1" ]] 
then 
    ddir=$1
    if [[ ! -z "$2" ]] 
    then 
        ndir=$2
        if [[ ! -z "$3" ]] 
        then 
            pcddir=$3
        fi
    fi
fi
cd $ddir

declare -a arrD
for file in *0.png
do
    arrD=("${arrD[@]}" "$file")
done

cd $ndir
declare -a arrN
for file in *0_pred.png
do
    arrN=("${arrN[@]}" "$file")
done

cd $builddir

for i in "${!arrD[@]}"; do
    dfilename="${arrD[i]}"
    nfilename="${arrN[i]}"
    echo $ddir$dfilename
    # echo $ndir$nfilename
    ./depth2pcd_normal $ddir $ndir $pcddir $dfilename $nfilename $camera
done