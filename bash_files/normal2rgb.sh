#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/lenssen_tofnest/
# basedir=/media/rambo/ssd2/Szilard/nyu_tofnest/
# basedir=/media/rambo/ssd2/Szilard/pico_tofnest/
input_dir=$basedir'pcdnormals/' # path to input file directory
output_dir=$basedir'pcdrgb/' # path to input file directory

if [[ ! -z "$1" ]] 
then 
    input_dir=$1
    if [[ ! -z "$2" ]] 
    then 
        output_dir=$2
    fi
fi
cd $input_dir

for filename in *280.pcd; do # or for filename in *normals.pcd; do
    cd $builddir
    ./normal2rgb $input_dir $output_dir $filename
done