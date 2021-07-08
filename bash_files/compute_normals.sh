#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/c24/640x480/
# basedir=/media/rambo/ssd2/Szilard/pico_tofnest/
# basedir=/media/rambo/ssd2/Szilard/nyu_tofnest/
input_dir=$basedir'pcd/'
output_dir=$basedir'pcdnormals/'
normalradius=1.0

if [[ ! -z "$1" ]] 
then 
    input_dir=$1
    if [[ ! -z "$2" ]] 
    then 
        output_dir=$2
    fi
fi
cd $input_dir

for filename in 00{5,6,7}*.pcd; do
    cd $builddir
    ./compute_normals $input_dir $output_dir $filename $normalradius
done