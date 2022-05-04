import cv2
import numpy as np
import os
import argparse
import open3d as o3d
import trimesh

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--dir', default="",
                    help='the directory to the source files')
parser.add_argument('--size', default=32,
                    help='the size of the gim, it will be squared')
args = parser.parse_args()
directory=args.dir
arrays=[]
print(directory)
dlist=os.listdir(directory)
dlist.sort()
for filename in dlist:
    if filename.endswith(".npy"):
        arrays.append(filename)
    else:
        continue

for i in range(len(arrays)):    
    # 
    filename=arrays[i]
    nparray = np.load(directory+filename)
    print(nparray.shape)
    print(nparray.max())
    print(nparray.mean())
    nparray[nparray==nparray.max()]=0
    nparray*=1000
    print(nparray.max())
    print(nparray.mean())
    filename_gim = filename[:-3]+"png"
    print(filename)
    cv2.imwrite(directory+filename_gim,nparray.astype(np.uint16))
