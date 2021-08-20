import h5py
import os
import numpy as np
import imageio

dir='/home/szilard/Downloads/nyuv2_surfacenormal_metadata/official/'
dlist=os.listdir(dir)
dlist.sort()
files=[]
for filename in dlist:
    if filename.endswith(".h5"):
        #print(os.path.join(directory, filename))
        files.append(filename)
    else:
        continue
    
print(len(files))
for i in range(len(files)):
    number=f'{i:06d}'
    with h5py.File(dir+files[i], "r") as f:
        # List all groups
        # print("Keys: %s" % f.keys())
        a_group_key = list(f.keys())[0]

        # Get the data
        data = list(f[a_group_key])
        im=np.array(data,dtype=np.uint16)
        imageio.imwrite(dir+number+'_depth.png',im) 
        print(i)