# program to make a dictionary tree from the syncingFolder structure

import os
import sys
import json

def makeTree(path):
    tree = []
    for i in os.listdir(path):
        if os.path.isfile(path+"/"+i):
            tree.append(i)
        else:
            tree.append({i: makeTree(path+"/"+i)})
    return tree

def makeList(tree, path):
    data = []
    for i in tree:
        if type(i) == str:
            data.append(path+"/"+i)
        else:
            data += makeList(i[list(i.keys())[0]], path+"/"+list(i.keys())[0])
    return data

def writeTime(l: list):
    return [{"path": i, "time": int(os.path.getmtime(i))} for i in l]

if __name__ == "__main__":
    print(json.dumps(writeTime(makeList(makeTree("."), ".")), indent=4))
