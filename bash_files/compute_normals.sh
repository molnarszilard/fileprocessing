#!bin/bash
builddir=$PWD'/../build/'
# basedir=/media/rambo/ssd2/Szilard/c24/
# basedir=/media/rambo/ssd2/Szilard/pico_tofnest/
# basedir=/media/rambo/ssd2/Szilard/nyu_tofnest/
basedir=/media/rambo/ssd2/Szilard/kitti/validation/
input_dir=$basedir'pcddense/'
output_dir=$basedir'pcdnormals/'
normalradius=0.5

if [[ ! -z "$1" ]] 
then 
    input_dir=$1
    if [[ ! -z "$2" ]] 
    then 
        output_dir=$2
    fi
fi
cd $input_dir

for filename in *.pcd; do
    cd $builddir
    ./compute_normals $input_dir $output_dir $filename $normalradius
done