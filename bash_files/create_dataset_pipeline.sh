#!bin/bash
bashdir=$PWD
builddir=$PWD'/../build/'
# evaldir=$PWD'/../../dataset/evaluation/'
work_dir='/media/rambo/ssd2/Szilard/toffilter_nyu/'
filter_type='sor'
if [[ ! -z "$1" ]] 
then 
    work_dir=$1
    if [[ ! -z "$2" ]] 
    then 
        filter_type=$2
    fi
fi
cd $work_dir
# rm -rf depth_aug/
# mkdir depth_aug
rm -rf pcd_aug
mkdir pcd_aug
rm -rf pcd_aug_filtered
mkdir pcd_aug_filtered
rm -rf depth_aug_filtered
mkdir depth_aug_filtered
rm -rf dataset
mkdir dataset
mkdir dataset/gt
mkdir dataset/noisy
mkdir dataset/gt/test
mkdir dataset/noisy/test
mkdir dataset/gt/train
mkdir dataset/noisy/train

# cp depth/* depth_aug/
cd $bashdir
# python3 augmentation.py --dir=$work_dir'depth_aug/'
# python3 rename.py --dir=$work_dir'depth_aug/' --ext=.png

bash depth2pcd.sh $work_dir'depth_aug/' $work_dir'pcd_aug/'
case $filter_type in
    sor)
        bash sor.sh $work_dir'pcd_aug/' $work_dir'pcd_aug_filtered/'
        ;;
    voxel)
        bash voxel_filter.sh $work_dir'pcd_aug/' $work_dir'pcd_aug_filtered/'
        ;;
    *)
        bash sor.sh $work_dir'pcd_aug/' $work_dir'pcd_aug_filtered/'
        ;;
esac
bash pcd2depth.sh $work_dir'pcd_aug_filtered/' $work_dir'depth_aug_filtered/'
python3 rename.py --dir=$work_dir'depth_aug_filtered/' --ext=.png

cd $work_dir
cp depth_aug/* dataset/noisy/
cp depth_aug_filtered/* dataset/gt/

cd $bashdir
python3 create_train.py --dir=$work_dir'dataset/gt/'
python3 create_train.py --dir=$work_dir'dataset/noisy/'