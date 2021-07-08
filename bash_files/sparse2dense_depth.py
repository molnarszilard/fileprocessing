from PIL import Image
import argparse

from numpy.lib.arraypad import pad
import cv2
import numpy as np
import os

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--dir', default="/media/rambo/ssd2/Szilard/kitti/tofnest/depth/",
                    help='the directory to the source files')
parser.add_argument('--dirout', default="/media/rambo/ssd2/Szilard/kitti/tofnest/depth_dense/",
                    help='the directory to the output files')
args = parser.parse_args()

directory=args.dir
directorymod=args.dirout
images=[]

dlist=os.listdir(directory)
dlist.sort()
for filename in dlist:
    if filename.endswith(".png"):
        #print(os.path.join(directory, filename))
        images.append(filename)
    else:
        continue

# print(len(images))

def method1(mat):
    zero = True
    
    while zero:
        nrzero = 0
        zero = False
        for i in range(len(mat)):
            for j in range(len(mat[1])):
                if mat[i][j]>0:
                    if i>0:
                        if mat[i-1][j]==0:
                            mat[i-1][j]=mat[i][j]
                    if i<len(mat)-1:
                        if mat[i+1][j]==0:
                            mat[i+1][j]=mat[i][j]
                    if j>0:
                        if mat[i][j-1]==0:
                            mat[i][j-1]=mat[i][j]
                    if j<len(mat[1])-1:
                        if mat[i][j+1]==0:
                            mat[i][j+1]=mat[i][j]
                    if i>0 and j>0:
                        if mat[i-1][j-1]==0:
                            mat[i-1][j-1]=mat[i][j]
                    if i<len(mat)-1 and j>0:
                        if mat[i+1][j-1]==0:
                            mat[i+1][j-1]=mat[i][j]
                    if i>0 and j<len(mat[1])-1:
                        if mat[i-1][j+1]==0:
                            mat[i-1][j+1]=mat[i][j]
                    if i<len(mat)-1 and j<len(mat[1])-1:
                        if mat[i+1][j+1]==0:
                            mat[i+1][j+1]=mat[i][j]
                else:
                    zero = True
                    nrzero=nrzero+1
        print(nrzero)

    return mat            

def method2(mat):
    padding_size=3
    nrzero=0
    for i in range(len(mat)):
        for j in range(len(mat[1])):
            zero=False
            if mat[i,j]==0:
                nrzero=nrzero+1
                i2=i+padding_size
                j2=j+padding_size
                mat2=np.pad(mat, 2,  'constant', constant_values=(0))
                # print(mat2[i2-padding_size:i2+padding_size+1,j2-padding_size:j2+padding_size+1])
                # print(i)
                mat[i,j]=np.mean(mat2[i2-padding_size:i2+padding_size+1,j2-padding_size:j2+padding_size+1])
    # print(nrzero)
    return mat

for i in range(len(images)):
    print(images[i])
    if i<19:
        continue
    sparse=cv2.imread(directory+images[i],cv2.IMREAD_UNCHANGED)
    # dense = np.zeros((len(sparse),len(sparse[0])), 'uint16')
    dense = method2(sparse)    
    im = Image.fromarray(np.uint16(dense))
    #image.show()
    im.save(directorymod+images[i])