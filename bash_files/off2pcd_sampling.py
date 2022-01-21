import os
import glob
import trimesh
import numpy as np
import pclpy 
from pclpy import pcl

DATA_DIR = "/mnt/ssd1/datasets/"
DATA_DIR = os.path.join(os.path.dirname(DATA_DIR), "ModelNet10")
num_points=10000
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
        if f.endswith("off"):
            print("Current file: " + f)
            train_points=trimesh.load(f).sample(num_points)
            train_np=np.array(train_points).squeeze()
            pc2 = pcl.PointCloud.PointXYZ(train_np)
            writer = pcl.io.PCDWriter()
            writer.writeBinary(f[:-3]+"pcd",pc2)

    for f in test_files:
        if f.endswith("off"):
            print("Current file: " + f)
            test_points=trimesh.load(f).sample(num_points)
            test_np=np.array(test_points).squeeze()
            pc2 = pcl.PointCloud.PointXYZ(test_np)
            writer = pcl.io.PCDWriter()
            writer.writeBinary(f[:-3]+"pcd",pc2)
