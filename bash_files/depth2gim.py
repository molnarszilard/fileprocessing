import cv2
import numpy as np
import os
import argparse
import open3d as o3d
import trimesh

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--dir', default="",
                    help='the directory to the source files')
parser.add_argument('--size', default=32,type=int,
                    help='the size of the gim, it will be squared')
args = parser.parse_args()

directory=args.dir
depths=[]
print(directory)
dlist=os.listdir(directory)
dlist.sort()
for filename in dlist:
    if filename.endswith(".jpg") or filename.endswith(".png"):
       depths.append(filename)
    else:
        continue

for i in range(len(depths)):    
    # 
    filename=depths[i]
    depth=cv2.imread(directory+filename,cv2.IMREAD_UNCHANGED)
    # print(depth.max())
    # print(depth.mean())
    # depth[depth>=depth.max()]=0
    # print(depth.max())
    # print(depth[depth>0].mean())
    # print(depth[240,320])
    # depth = np.reshape(depth,(512,600))
    print(depth.shape)
    # depth = np.resize(depth,(512,512))
    # print(depth.shape)
    # height_plus = int((depth.shape[0]-256)/2)
    # width_plus = int((depth.shape[1]-256)/2)
    # # print(height_plus)
    # # print(width_plus)
    # depth_crop = depth[height_plus:height_plus+256,width_plus:width_plus+256]
    # filename_depth = filename[:-4]+"_cropped.png"
    # cv2.imwrite(directory+filename_depth,depth_crop)
    # print(depth_crop.shape)
    K = [579.8181762695312, 0.0, 320.0, 0.0, 579.8181762695312, 240.0, 0.0, 0.0, 1.0]
    fx = K[0]
    fy = K[4]
    x0 = K[2]
    y0 = K[5]
    pcddepth = []
    print(len(pcddepth))
    for i in range(depth.shape[0]):
        for j in range(depth.shape[1]):
            if True:
                index=i*depth.shape[1]+j
                d = depth[i, j]/1000.0
                if d == 0.0:
                    continue
                # else:
                #     print(d)
                x_over_z = (j - x0) / fx
                y_over_z = (i - y0) / fy
                array = [x_over_z * d,y_over_z * d,d]
                pcddepth.append(array)
                # pcddepth[index][2] = d
                # pcddepth[index][0] = x_over_z * d
                # pcddepth[index][1] = y_over_z * d
    print(len(pcddepth))
    pcddepth = np.resize(pcddepth,(len(pcddepth),3))
    pcddepth = pcddepth-pcddepth.min()
    pcddepth = pcddepth/pcddepth.max()
    pcd = o3d.geometry.PointCloud()
    pcd.points = o3d.utility.Vector3dVector(pcddepth)
    filename_pcd = filename[:-3]+"pcd"
    o3d.io.write_point_cloud(directory+filename_pcd,pcd)
    # pcd = pcd.uniform_down_sample(4)
    print(len(pcd.points))
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
    test_points=tri_mesh.sample(args.size*args.size)
    pcdnp=np.array(test_points).squeeze()
    pcd.points = o3d.utility.Vector3dVector(pcdnp)
    filename_pcd = filename[:-4]+"_down.pcd"
    o3d.io.write_point_cloud(directory+filename_pcd,pcd)
    pcdnp = np.asarray(pcd.points)
    # print(pcdnp.shape)
    # depthgim=np.zeros((32,32,3))
    # for i in range(pcdnp.shape[0]):
    #     z = pcdnp[i][2] * 1000.0
    #     u = (pcdnp[i][0] * 1000.0 * fx) / z
    #     v = (pcdnp[i][1] * 1000.0 * fy) / z
    #     pixel_pos_x = round(u + x0)
    #     pixel_pos_y = round(v + y0)

    #     if (pixel_pos_x < 0):
    #         pixel_pos_x = -pixel_pos_x
    #     if (pixel_pos_x > (32 - 1)):
    #         pixel_pos_x = 32 - 1

    #     if (pixel_pos_y < 0):
    #         pixel_pos_y = -pixel_pos_y
    #     if (pixel_pos_y > (32 - 1)):
    #         pixel_pos_y = 32 - 1
    #     depthgim[pixel_pos_y][pixel_pos_x][2] = pcdnp[i][2]
    #     depthgim[pixel_pos_y][pixel_pos_x][0] = pcdnp[i][0]
    #     depthgim[pixel_pos_y][pixel_pos_x][1] = pcdnp[i][1]
    #     # output.at<uint16_t>(pixel_pos_y, pixel_pos_x) = z
    # depthgim*=255
    # depthgim = depthgim.astype(np.uint8)
    # filename_gim = filename[:-4]+"_gim.png"
    # print(filename)
    # cv2.imwrite(directory+filename_gim,depthgim)

    pcdnp*=255
    pcdnp = pcdnp.astype(np.uint8)
    gim = np.reshape(pcdnp,(args.size,args.size,3))
    filename_gim = filename[:-4]+"_gim.png"
    print(filename)
    cv2.imwrite(directory+filename_gim,gim)

    gim2flat = np.reshape(gim/255,(args.size*args.size,3))
    pcd = o3d.geometry.PointCloud()
    pcd.points = o3d.utility.Vector3dVector(gim2flat)
    filename_pcd = filename[:-4]+"_gim.pcd"
    o3d.io.write_point_cloud(directory+filename_pcd,pcd)