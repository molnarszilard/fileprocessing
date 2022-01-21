import open3d as o3d
import trimesh
import numpy as np
import os
import glob
from pclpy import pcl

DATA_DIR = "/mnt/ssd1/datasets/"
DATA_DIR = os.path.join(os.path.dirname(DATA_DIR), "ModelNet10_pcd")
num_points=2048
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
            pcd = o3d.io.read_point_cloud(f)
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
            writer = pcl.io.PCDWriter()
            writer.writeBinary(f[:-4]+"_s.pcd",pc2)

    for f in test_files:
        if f.endswith("pcd"):
            print("Current file: " + f)
            pcd = o3d.io.read_point_cloud(f)
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
            test_points=tri_mesh.sample(num_points)
            test_np=np.array(test_points).squeeze()
            pc2 = pcl.PointCloud.PointXYZ(test_np)
            writer = pcl.io.PCDWriter()
            writer.writeBinary(f[:-4]+"_s.pcd",pc2)