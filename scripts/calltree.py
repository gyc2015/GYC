#!/usr/bin/python

import os,re

def GenCallTree(srcDir,dstDir):
    srcItemList = os.listdir(srcDir)
    for item in srcItemList:
        itemSrcName = os.path.join(srcDir,item)
        itemDstName = os.path.join(dstDir,item)
        if os.path.isdir(itemSrcName):
            if not os.path.exists(itemDstName):
                os.mkdir(itemDstName)
            GenCallTree(itemSrcName,itemDstName)
        elif os.path.isfile(itemSrcName):
            fileName = os.path.splitext(itemDstName)
            if '.c' == fileName[1]:
                os.system('./gyc.exe '+itemSrcName + ' > ' + fileName[0] + '.ct')


srcDir = "."
dstDir = "."

GenCallTree(srcDir,dstDir)
os.system('./network_properties.exe .')


