import os
import numpy as np
import argparse
import cv2

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--dir', default="/media/rambo/ssd2/Szilard/kitti/depth_dense/",
                    help='the directory to the source files')
parser.add_argument('--h', default=350,type=int,
                    help='the height of the resulting image')
parser.add_argument('--w', default=1200,type=int,
                    help='the width of the resulting image')
args = parser.parse_args()

directory=args.dir
images=[]
imgs=[]
dlist=os.listdir(directory)
dlist.sort()
for filename in dlist:
    if filename.endswith(".jpg") or filename.endswith(".png"):
        #print(os.path.join(directory, filename))
        images.append(filename)
    else:
        continue

print(len(images))

for i in range(len(images)):
    image_name=images[i]
    print("Image:"+image_name)
    depth = cv2.imread(directory+image_name,cv2.IMREAD_UNCHANGED )
    depth = cv2.resize(depth, dsize=(args.w, args.h), interpolation=cv2.INTER_NEAREST)
    cv2.imwrite(directory+image_name, depth)
