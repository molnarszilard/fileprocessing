#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/lenssen_tofnest/
input_dir=$basedir'noise/pcd_n02/'
output_dir=$basedir'noise/xyz_n02/'

if [[ ! -z "$1" ]] 
then 
    input_dir=$1
    if [[ ! -z "$2" ]] 
    then 
        output_dir=$2
    fi
fi
cd $input_dir

for filename in *s.pcd; do
    cd $builddir
    ./pcd2xyz $input_dir $output_dir $filename
done