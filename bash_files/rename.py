
import os
import argparse
import shutil


parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--dir', default="/media/rambo/ssd2/Szilard/nyu_v2_filter/pcn_or/info/",
                    help='the directory to the source files')
parser.add_argument('--ext', default=".pcd",
                    help='the extension of file')
parser.add_argument('--ending', default=".pcd",
                    help='the ending name of a file')
parser.add_argument('--number', default=0, type=int,
                    help='start of the numbering')
parser.add_argument('--copy', default=False, type=bool,
                    help='move or copy')
parser.add_argument('--increment', default=1, type=int, 
                    help='how much is the increment, 1,10 ....')
args = parser.parse_args()

directory=args.dir

dlist=os.listdir(directory)
dlist.sort()
extension = args.ext

n=args.number
for filename in dlist:
    if filename.endswith(extension):
        number=f'{n:05d}'
        if args.copy:
            shutil.copy2(directory+filename,directory+number+args.ending)
        else:
            shutil.move(directory+filename,directory+number+args.ending)
        n=n+args.increment
    else:
        continue