#!bin/bash
builddir=$PWD'/../build/'
ddir=/media/rambo/ssd2/Szilard/nyu_v2_filter/comparison/own/working/depthpred/
ndir=/media/rambo/ssd2/Szilard/nyu_v2_filter/comparison/own/working/normal/
pcddir=/media/rambo/ssd2/Szilard/nyu_v2_filter/comparison/own/working/pcdpred/

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
for file in *.png
do
    arrD=("${arrD[@]}" "$file")
done

cd $ndir
declare -a arrN
for file in *.png
do
    arrN=("${arrN[@]}" "$file")
done

cd $builddir

for i in "${!arrD[@]}"; do
    dfilename="${arrD[i]}"
    nfilename="${arrN[i]}"
    echo $ndir$dfilename
    ./depth2pcd_normal $ddir $ndir $pcddir $dfilename $nfilename kitti
done