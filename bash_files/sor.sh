#!bin/bash
#!bin/bash
builddir=$PWD'/../build/'
input_dir=/media/rambo/ssd2/Szilard/toffilter_isaac/pcd/
output_dir=/media/rambo/ssd2/Szilard/toffilter_isaac/pcd_sor/

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
    # input directory, output directory, filename, meanK, stddevMultiTresh
    ./sor $input_dir $output_dir $filename 50 0.5
done