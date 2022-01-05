#!bin/bash
builddir=$PWD'/../build/'
input_dir=/mnt/ssd1/datasets/ModelNet40/cone/train/
output_dir=/mnt/ssd1/datasets/ModelNet40pcd/cone/train/

if [[ ! -z "$1" ]] 
then 
    input_dir=$1
    if [[ ! -z "$2" ]] 
    then 
        output_dir=$2
    fi
fi
cd $input_dir

for filename in *.off; do
    cd $builddir
    ./off2pcd $input_dir $output_dir $filename
done