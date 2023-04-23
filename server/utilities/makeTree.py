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

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python makeTree.py <path>")
        sys.exit(1)
    path = sys.argv[1]
    tree = makeTree(path)
    print(json.dumps(tree, indent=4))
