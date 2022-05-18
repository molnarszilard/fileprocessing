#!bin/bash
builddir=$PWD'/../build/'

# path, leaf size, meanK, StddevMulThresh
histo=histogram_test_d2ir.txt
start=0
end=4331
# basedir=/media/rambo/ssd2/Szilard/kitti/validation/
# basedir=/media/rambo/ssd2/Szilard/c24/
# basedir=/media/rambo/ssd2/Szilard/pico_tofnest/
# basedir=/media/rambo/ssd2/Szilard/nyu_tofnest/
basedir=/mnt/ssd1/datasets/c24/evaluation/
gtdir=$basedir'pcdnormal/'
deltadir=$basedir'pcdpred_delta_d2ir/'
gtending=.pcd
step=10

case $1 in
    own) # ToFNest
        echo ToFNest
        preddir=$basedir'pcdpred_d2ir/'
        predending=.pcd
        ;;
    nesti) # Nesti-Net
        echo Nesti-Net
        preddir=/media/rambo/ssd2/Szilard/lenssen_tofnest/comp/
        predending=_nestinormals.pcd
        ;;
    pcpss) # PCPNet single scale
        echo PCPNet_ss
        preddir=$basedir'noise/pcpnetgen_n02/single_scale_normal/'
        predending=_pcpnetnormals.pcd
        ;;
    pcpms) #PCPNet multi scale
        echo PCPNet_ms
        preddir=$basedir'pcpnetms/multi_scale_normal/'
        predending=.pcd
        ;;
    pcl) # Point CLoud Normals
        echo PCLNORMALS
        preddir=/media/rambo/ssd2/Szilard/lenssen_tofnest/comp/
        predending=_pclouds_normals_pclnormals.pcd
        ;;
    hough) # Hough Normals
        echo HOUGHNORMALS
        preddir=/media/rambo/ssd2/Szilard/lenssen_tofnest/comp/
        predending=_houghnormals.pcd
        ;;
    ladicky) # Hough Normals
        echo LADICKY
        preddir=/media/rambo/ssd2/Szilard/nyu_tofnest/pcdnormals/
        predending=.pcd
        ;;
    *) # toffilter
        echo Undefined, ToFNest
        preddir=$basedir'pcdpred/'
        predending=_pred.pcd
        ;;    
esac
# echo $preddir
cd $builddir
dont_save_delta_pcd=no
case $2 in
    quality)
        ./normal_performance_quality $histo $gtdir $preddir $gtending $predending $dont_save_delta_pcd $start $end $step
        ;;
    quality_delta)
        ./normal_performance_quality $histo $gtdir $preddir $gtending $predending $deltadir $start $end $step
        ;;
    angle)
        ./normal_performance_angle $histo $gtdir $preddir $gtending $predending $start $end $step
        ;;
    distribution)
        ./normal_performance_distribution $gtdir $preddir $gtending $predending $start $end $step
        ;;
    *)
        ./normal_performance_quality $histo $gtdir $preddir $gtending $predending $deltadir $start $end $step
        ;;
esac

