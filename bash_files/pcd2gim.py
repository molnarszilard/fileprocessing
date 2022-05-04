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
clouds=[]
print(directory)
dlist=os.listdir(directory)
dlist.sort()
for filename in dlist:
    if filename.endswith(".pcd"):
        clouds.append(filename)
    else:
        continue

for i in range(len(clouds)):    
    # 
    filename=clouds[i]
    pcd = o3d.io.read_point_cloud(directory+filename,remove_nan_points=True,remove_infinite_points=True)
    pcd = pcd.uniform_down_sample(2)
    pcdnp = np.asarray(pcd.points)
    pcdnp*=255
    pcdnp = pcdnp.astype(np.uint8)
    gim = np.reshape(pcdnp,(args.size,args.size,3))
    filename_gim = filename[:-3]+"png"
    print(filename)
    cv2.imwrite(directory+filename_gim,gim)
