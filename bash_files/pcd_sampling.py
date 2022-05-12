import open3d as o3d
import trimesh
import numpy as np
import os
import glob
import argparse
from pclpy import pcl

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--dir', default="/mnt/ssd1/datasets/c24/pcd_4bag/",
                    help='the directory to the source files')
parser.add_argument('--size', default=4096, type=int,
                    help='the size of the new pcd')
parser.add_argument('--multiple_folders', default=False,
                    help='if you have multiple subfolder containing point clouds, set this flag to true')
args = parser.parse_args()
DATA_DIR = args.dir
num_points=args.size

def convert(pcd):
    pcd = pcd.uniform_down_sample(2)
    pcd.estimate_normals()
    # estimate radius for rolling ball
    distances = pcd.compute_nearest_neighbor_distance()
    avg_dist = np.mean(distances)
    radius = 1.5 * avg_dist   

    mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_ball_pivoting(
            pcd,
            o3d.utility.DoubleVector([radius, radius * 2]))

    # create the triangular mesh with the vertices and faces from open3d
    tri_mesh = trimesh.Trimesh(np.asarray(mesh.vertices), np.asarray(mesh.triangles),
                            vertex_normals=np.asarray(mesh.vertex_normals))

    trimesh.convex.is_convex(tri_mesh)
    train_points=tri_mesh.sample(num_points)
    train_np=np.array(train_points).squeeze()
    pc2 = pcl.PointCloud.PointXYZ(train_np)
    return pc2

if args.multiple_folders:
    class_map = {}
    folders = glob.glob(os.path.join(DATA_DIR, "[!README]*"))
    for i, folder in enumerate(folders):
        print("processing class: {}".format(os.path.basename(folder)))
        # store folder name with ID so we can retrieve later
        class_map[i] = folder.split("/")[-1]
        # gather all files
        train_files = glob.glob(os.path.join(folder, "train/*"))
        test_files = glob.glob(os.path.join(folder, "test/*"))

        for f in train_files:
            if f.endswith("pcd"):
                print("Current file: " + f)
                pcd = o3d.io.read_point_cloud(f,remove_nan_points=True,remove_infinite_points=True)
                pc2 = convert(pcd)
                writer = pcl.io.PCDWriter()
                writer.writeBinary(f[:-4]+"_s.pcd",pc2)

        for f in test_files:
            if f.endswith("pcd"):
                print("Current file: " + f)
                pcd = o3d.io.read_point_cloud(f,remove_nan_points=True,remove_infinite_points=True)
                pc2 = convert(pcd)
                writer = pcl.io.PCDWriter()
                writer.writeBinary(f[:-4]+"_s.pcd",pc2)

else:
    print(DATA_DIR)
    dlist=os.listdir(DATA_DIR)
    dlist.sort()
    for f in dlist:    
        if f.endswith("pcd"):
            print("Current file: " + f)
            pcd = o3d.io.read_point_cloud(DATA_DIR+f,remove_nan_points=True,remove_infinite_points=True)
            pc2 = convert(pcd)
            writer = pcl.io.PCDWriter()
            writer.writeBinary(DATA_DIR+f[:-4]+"_sampled.pcd",pc2)
