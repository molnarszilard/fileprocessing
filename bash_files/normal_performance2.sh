#!bin/bash
builddir=$PWD'/../build/'

# path, leaf size, meanK, StddevMulThresh
histo=histogram_lenssenn_on_kittid.txt
start=0
end=1000
basedir=/media/rambo/ssd2/Szilard/kitti/validation/
# basedir=/media/rambo/ssd2/Szilard/c24/
# basedir=/media/rambo/ssd2/Szilard/pico_tofnest/
# basedir=/media/rambo/ssd2/Szilard/nyu_tofnest/
# basedir=/media/rambo/ssd2/Szilard/lenssen_tofnest/
gtdir=$basedir'pcdnormals/'
deltadir=$basedir'pcdpred_delta/'
gtending=.pcd
step=10
camera="kitti" #pico, nyu, kitti, isaac

case $1 in
    own) # ToFNest
        echo ToFNest
        preddir=$basedir'pcdpred/'
        predending=.pcd
        ;;
    nesti) # Nesti-Net
        echo Nesti-Net
        preddir=/media/rambo/ssd2/Szilard/nestigen/
        predending=_nestinormals.pcd
        ;;
    pcpss) # PCPNet single scale
        echo PCPNet_ss
        preddir=/media/rambo/ssd2/Szilard/pcpnetgen/single_scale_normal/
        predending=_pcpnetnormals.pcd
        ;;
    pcpms) #PCPNet multi scale
        echo PCPNet_ms
        preddir=/media/rambo/ssd2/Szilard/pcpnetgen/multi_scale_normal/
        predending=_pcpnetnormals.pcd
        ;;
    pcl) # Point CLoud Normals
        echo PCLNORMALS
        preddir=/media/rambo/ssd2/Szilard/pclnormals/
        predending=_pclouds_normals_pclnormals.pcd
        ;;
    hough) # Hough Normals
        echo HOUGHNORMALS
        preddir=/media/rambo/ssd2/Szilard/houghgen/
        predending=_houghnormals.pcd
        ;;
    *) # toffilter
        echo Undefined, ToFNest
        preddir=$basedir'pcdpred/'
        predending=_pred.pcd
        ;;    
esac
# echo $preddir
cd $builddir
./normal_performance_quality2 $histo $gtdir $preddir $gtending $predending $start $end $step $camera


