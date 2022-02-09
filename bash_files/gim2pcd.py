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
        print(filename)
    else:
        continue

if not os.path.exists(directory+"test"):
    os.makedirs(directory+"test")
if not os.path.exists(directory+"train"):
    os.makedirs(directory+"train")      
# file1 = open(directory+"filelist.txt" ,'r')
# Lines = file1.readlines()
n=0

for i in range(len(images)):    
    # 
    filename=images[i]
    image=cv2.imread(directory+filename,cv2.IMREAD_UNCHANGED)
    filename_pcd = filename[:-3]+"pcd"
    print(filename)
    gimnp=np.array(image)
    gim2flat = np.array([gimnp[0].flatten(),gimnp[1].flatten(),gimnp[2].flatten()])
    gim2flat = np.moveaxis(gim2flat,0,-1)
    # pcd = o3d.geometry.PointCloud()
    # pcd.points = o3d.utility.Vector3dVector(gim2flat)
    # pcd.io.write_point_cloud(directory+filename_pcd, write_ascii=True)
    # pcd.estimate_normals()
    # # estimate radius for rolling ball
    # distances = pcd.compute_nearest_neighbor_distance()
    # avg_dist = np.mean(distances)
    # radius = 1.5 * avg_dist
    # mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_ball_pivoting(
    #         pcd,
    #         o3d.utility.DoubleVector([radius, radius * 2]))
    # # create the triangular mesh with the vertices and faces from open3d
    # tri_mesh = trimesh.Trimesh(np.asarray(mesh.vertices), np.asarray(mesh.triangles),
    #                         vertex_normals=np.asarray(mesh.vertex_normals))

    # trimesh.convex.is_convex(tri_mesh)
    # points=tri_mesh.sample(2048)
    # pointsnp=np.array(points)
    # pointsnp = np.moveaxis(pointsnp,0,-1)
    # pointsnp = pointsnp - pointsnp.min()
    # pointsnp_normalized = pointsnp/np.absolute(pointsnp).max()-0.5

