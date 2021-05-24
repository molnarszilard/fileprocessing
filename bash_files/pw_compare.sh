#!bin/bash
builddir=$PWD'/../build/'

# path, leaf size, meanK, StddevMulThresh
histo=histogram_$1.txt
start=0
end=1449
gtdir=/media/rambo/ssd2/Szilard/toffilter_nyu/evaluation/$2gt/
gtending=.pcd

case $1 in
    base) # original noisy data
    echo BASE
        preddir=/media/rambo/ssd2/Szilard/toffilter_nyu/evaluation/$2/
        predending=.pcd
        ;;
    own) # toffilter prediction
        echo ToFFilter
        preddir=/media/rambo/ssd2/Szilard/toffilter_nyu/evaluation/$2_pred/
        predending=.pcd
        ;;
    ownmask) # toffilter prediction with mask
        echo ToFFilterMask
        preddir=/media/rambo/ssd2/Szilard/nyu_v2_filter/comparison/own/pcdmasked/$noise/
        predending=_mask.pcd
        ;;
    *) # toffilter
        echo Undefined, ToFFilter
        preddir=/media/rambo/ssd2/Szilard/nyu_v2_filter/comparison/own/pcdpred_$noise/
        predending=.pcd
        ;;    
esac
# echo $preddir
cd $builddir
step=1
case $2 in
    depth)
        ./pw_depthcompare $gtdir $preddir $gtending $predending $start $end $step
        ;;
    pcd)
        echo pcd
        ./pw_pcdcompare $gtdir $preddir $gtending $predending $start $end $step nyu 480 640
        ;;
    *)
        ./pw_depthcompare $gtdir $preddir $gtending $predending $start $end $step
        ;;
esac