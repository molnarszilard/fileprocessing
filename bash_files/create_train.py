import cv2
import numpy as np
import os
import argparse
import shutil

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--dir', default="",
                    help='the directory to the source files')
parser.add_argument('-p','--proportion', default=5, type=int,
                    help='every n-th image is considered test, others are considered train')
parser.add_argument('--ending', default="", type=str,
                    help='check the file format what you would like to check. By default it works with png,jpg,npy and pcd')
args = parser.parse_args()

directory=args.dir
images=[]

dlist=os.listdir(directory)
dlist.sort()
for filename in dlist:
    if args.ending=="":
        if filename.endswith(".jpg") or filename.endswith(".png") or filename.endswith(".npy") or filename.endswith(".pcd"):
            #print(os.path.join(directory, filename))
            images.append(filename)
        else:
            continue
    else: 
        if filename.endswith(args.ending):
            #print(os.path.join(directory, filename))
            images.append(filename)
        else:
            continue

if not os.path.exists(directory+"test"):
    os.makedirs(directory+"test")
if not os.path.exists(directory+"train"):
    os.makedirs(directory+"train")      
# file1 = open(directory+"filelist.txt" ,'r')
# Lines = file1.readlines()
n=0

# for line in Lines:
#     # image=cv2.imread(directory+images[i],cv2.IMREAD_UNCHANGED )
#     image_name=line.strip()
#     filename=f'{n:05d}'
#     print(line.strip())
#     if n%3 == 0 :
#         # cv2.imwrite(directory+"test/"+filename+".png", image.astype(np.uint16)) 
#         shutil.copy2(directory+image_name,directory+"test/"+filename+".png")
#     else:
#         # cv2.imwrite(directory+"train/"+filename+".png", image.astype(np.uint16))
#         shutil.copy2(directory+image_name,directory+"train/"+filename+".png")
#     n=n+1

for i in range(len(images)):    
    # image=cv2.imread(directory+images[i],cv2.IMREAD_UNCHANGED )
    image_name=images[i]
    if n%args.proportion == 0 :
        # cv2.imwrite(directory+"test/"+filename+".png", image.astype(np.uint16)) 
        shutil.move(directory+image_name,directory+"test/"+image_name)
    else:
        # cv2.imwrite(directory+"train/"+filename+".png", image.astype(np.uint16))
        shutil.move(directory+image_name,directory+"train/"+image_name)
    n=n+1