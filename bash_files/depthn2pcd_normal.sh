#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/lenssen_tofnest/noise/
ddir=$basedir'depth/'
ndir=$basedir'pcpnetgen_n05/multi_scale_normal/'
pcddir=$basedir'pcpnetgen_n05/multi_scale_normal/'
camera="nyu" #pico, nyu, kitti, isaac
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
for file in *.normals
do
    arrN=("${arrN[@]}" "$file")
done

cd $builddir

for i in "${!arrD[@]}"; do
    dfilename="${arrD[i]}"
    nfilename="${arrN[i]}"
    ./depthn2pcd_normal $ddir $ndir $pcddir $dfilename $nfilename $camera
done