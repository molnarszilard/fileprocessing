#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/lenssen_tofnest/
input_dir=$basedir'pcdnormals/'
output_dir=$basedir'xyz/'

if [[ ! -z "$1" ]] 
then 
    input_dir=$1
    if [[ ! -z "$2" ]] 
    then 
        output_dir=$2
    fi
fi
cd $input_dir

for filename in *0.pcd; do
    cd $builddir
    ./pcd2xyz $input_dir $output_dir $filename
done