import cv2
import numpy as np
import os
import argparse
import open3d as o3d
import trimesh

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--dir', default="",
                    help='the directory to the source files')
args = parser.parse_args()

directory=args.dir
images=[]
print(directory)
dlist=os.listdir(directory)
dlist.sort()
for filename in dlist:
    if filename.endswith(".jpg") or filename.endswith(".png"):
        images.append(filename)
    else:
        continue

for i in range(len(images)):    
    # 
    filename=images[i]
    image=cv2.imread(directory+filename,cv2.IMREAD_UNCHANGED)
    filename_pcd = filename[:-3]+"pcd"
    print(filename)
    gimnp=np.array(image/255.0)
    length = 64*64
    if(image.shape[0]*image.shape[1]<length):
        length = 32*32
    gim2flat = np.reshape(gimnp,(length,3))
    pcd = o3d.geometry.PointCloud()
    pcd.points = o3d.utility.Vector3dVector(gim2flat)
    # pcd = pcd.uniform_down_sample(2)
    o3d.io.write_point_cloud(directory+filename_pcd,pcd)
    