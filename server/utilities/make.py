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
    l = []
    for i in tree:
        if type(i) == dict:
            l += makeList(i[list(i.keys())[0]], path+"/"+list(i.keys())[0])
        else:
            l.append(path+"/"+i)
    return l

if __name__ == "__main__":
    print(json.dumps(makeList(makeTree("."), "."), indent=4))
