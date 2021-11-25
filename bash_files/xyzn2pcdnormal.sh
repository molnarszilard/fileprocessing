#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/lenssen_tofnest/
xyzdir=$basedir'xyz/'
ndir=$basedir'noise/pcpnetgen_n02/single_scale_normal/'
pcddir=$basedir'noise/pcpnetgen_n02/single_scale_normal/'

if [[ ! -z "$1" ]] 
then 
    xyzdir=$1
fi
cd $xyzdir

declare -a arrXYZ
for file in 00000.xyz
do
    arrXYZ=("${arrXYZ[@]}" "$file")
done

cd $ndir
declare -a arrN
for file in *_.normals
do
    arrN=("${arrN[@]}" "$file")
done

cd $builddir

for i in "${!arrXYZ[@]}"; do
    xyzfilename="${arrXYZ[i]}"
    nfilename="${arrN[i]}"
    # echo $xyzfilename $nfilename
    ./xyzn2pcdnormal $xyzdir $ndir $pcddir $xyzfilename $nfilename
done