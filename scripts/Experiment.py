#!/usr/bin/env python
# -*- coding: utf-8

import re # 正则表达式
import os,sys
import filecmp # 文本比较
import random
import itertools

#  对比orcle
def FaultSelection(subjectProgram):
    subjectPath = r'/home/gyc/Documents/GYC/Linux对象/grep/outputs/'
    oraclePath = r'/home/gyc/Documents/GYC/Linux对象/grep/outputs.alt/' + subjectProgram + '/'
    # 记录成例和失例编号的列表
    trueRunList = []
    falseRunList = []
    # 读取oracle的输出
    oracleFiles = os.listdir(oraclePath)
    # 分别记录成例和失例编号的文件
    trueRunFile = open('./traces/a.txt','w+')
    falseRunFile = open('./traces/b.txt','w+')
    # 对比subject与oracle的输出文件
    for outfile in oracleFiles:
        oracleFile = oraclePath + outfile
        subjectFile = subjectPath + outfile
        # 获取测试用例编号
        testNum = GetTestNumber(outfile)
        # 判断subjectFile是否存在 若存在则对比文件，若不存在则判定相应用例为失例
        if os.path.isfile(subjectFile):
            # 文件存在，判断是否与oracle一致
            if filecmp.cmp(oracleFile, subjectFile):
                # 与oracle一致
                trueRunList.append(testNum)
                trueRunFile.write(str(testNum))
                trueRunFile.write('\n')
            else:
                # 与oracle不一致
                falseRunList.append(testNum)
                falseRunFile.write(str(testNum))
                falseRunFile.write('\n')
        else:
            # 文件不存在
            falseRunList.append(testNum)
            falseRunFile.write(str(testNum))
            falseRunFile.write('\n')
    trueRunFile.close()
    falseRunFile.close()
    # 文件对比完毕 拆分用例覆盖信息
    sucTraceFile = open('./traces/1_success_traces','w+')
    falTraceFile = open('./traces/1_crash_traces','w+')
    falseFile = open('./traces/false.txt','w+')
    # 对于1.txt中的每一行 即每个测试用例
    lineNumber = 0
    NumFalse = 0
    for line in open('./traces/1.txt'):
        m = IsACrashRun(line)
        if m:
            NumFalse = 0
            break
        else:
            lineNumber = lineNumber + 1
            s = str(lineNumber)
            if lineNumber in falseRunList:
                falTraceFile.write(line)
                falseFile.write(s + ' ')
                NumFalse = NumFalse + 1
            elif lineNumber in trueRunList:
                sucTraceFile.write(line)
    sucTraceFile.close()
    falTraceFile.close()
    falseFile.close()
    return NumFalse

# 获取测试用例编号
def GetTestNumber(outfile):
    if outfile[0:5] == 'out0.':
        result = int(outfile[5:])
    elif outfile == 'output.lex.c':
        result = 17
    elif outfile[0] == 'e':
        result = int(outfile[5:])
    elif outfile[0] == 'b':
        if outfile[7] == '1':
            result = 2
        elif outfile[7] == '2':
            result = 555
        elif outfile[7] == '3':
            result = 556
        elif outfile[7] == '4':
            result = 557
        else:
            print outfile
    elif outfile[0] == 't':
        result = int(outfile[1:])
    return result

# 获取输入注释的内容
def GetContaintofComment(comment):
    p = re.compile('(/\*)([ ,\t]*)(#[ ,\t]*(define)[ ,\t]*(.*))([ ,\t]*)(\*/)')
    m = p.match(comment)
    return m
def IsACrashRun(line):
    p = re.compile('crash\d+')
    m = p.match(line)
    return m
# 单缺陷实验
def SingleFaultExperiment(faultList,subject):
    # 遍历每个缺陷宏定义，生成头文件
    for faultComment in faultList:
        #  获取缺陷宏定义
        m = GetContaintofComment(faultComment)
        if m:
            #  宏定义 缺陷名称
            faultContaint = m.group(3)
            faultName = m.group(5)
            #  生成头文件
            dstFaultSeeds = file('FaultSeeds.h','w')
            dstFaultSeeds.write(faultContaint)
            dstFaultSeeds.close()
            #  执行脚本进行实验
            os.system('./' + subject + '.gcov.sh')
            #  对比拆分测试用例
            NumFailures = FaultSelection(subject)
            #  保存数据
            if not NumFailures == 0:
                os.system('mv traces ./' + subject + '/'+faultName)
            print faultContaint
        else:
            print 'No match'

# 判断选择的缺陷版本是否被禁
def IsForbidden(chosenFaults,forbiddenSetList):
    chosenFaultSet = set(chosenFaults)
    for forbiddenSet in forbiddenSetList:
        if forbiddenSet.issubset(chosenFaultSet):
            return True

# 多缺陷(2个)实验 超过了50个就推出
def Mul2FaultExperiment(faultList, subject, forbiddenSetList):
    # 生成组合
    com2 = list(itertools.combinations(faultList,2))
    faultMapFile = file(subject + '/faultMapFile', 'w')
    numMulVersions = 0
    timer = 0;
    # 对于每个组合
    for c in com2:
        numMulVersions = numMulVersions + 1
#        if 50 < timer:
#            break
        dstFaultSeeds = file('FaultSeeds.h','w')
        faultMap = 'Mul_2_' + str(numMulVersions)
        chosenFaults = []
        for item in c:
            # 获取缺陷宏定义
            m = GetContaintofComment(item)
            # 宏定义 缺陷名称
            faultContaint = m.group(3)
            chosenFaults.append(str(m.group(5)))
            faultMap = faultMap + ' ' + m.group(5)
            dstFaultSeeds.write(faultContaint + '\n')
        faultMapFile.write(faultMap + '\n')
        dstFaultSeeds.close()
        if not IsForbidden(chosenFaults,forbiddenSetList):            
            #  执行脚本进行实验
            os.system('./' + subject + '.gcov.sh')
            #  对比拆分测试用例
            NumFailures = FaultSelection(subject)
            #  保存数据
            if not NumFailures == 0:
                timer = timer + 1
                os.system('mv traces ./' + subject + '/Mul_2_'+str(numMulVersions))
        else:
            os.system('rm traces -r')
    faultMapFile.close()

# 多缺陷(n个)实验 超过了50个就推出
# faultList: 候选缺陷列表
# subject:   目标程序
# forbiddenSetList: 禁忌组合
# n:	一个版本中植入的缺陷数量
def MulnFaultExperiment(faultList, subject, forbiddenSetList,n):
    faultMapFile = file(subject + '/faultMapFile', 'a+')
    # 获取缺陷版本输出路径 和 缺陷版本目录
    versionPath = os.curdir + '/' + subject
    versionDirList = os.listdir(versionPath)
    # 生成组合 和 候选
    comn = list(itertools.combinations(faultList,n))
    comCandidates = list(range(0, len(comn)))
    # 从组合中去除已有的缺陷版本
    for comIndex in range(0,len(comn)):
        versionName = 'Mul_' + str(n) + '_' + str(comIndex + 1)
        if versionName in versionDirList:
            comCandidates.remove(comIndex)
    # 计时器 
    timer = 0;
    while len(comCandidates) > 0:
        # 超过了50次就推出
        if 50 < timer:
            break
        # 生成一个随机数 从comn中选取一个组合
        comIndex = random.randint(0,len(comCandidates)-1)
        numMulVersions = comCandidates[comIndex] + 1
        # 缺陷版本
        faultMap = 'Mul_' + str(n) + '_' +  str(numMulVersions)
        dstFaultSeeds = file('FaultSeeds.h','w')
        chosenFaults = []
        # 
        for item in comn[comCandidates[comIndex]]:
            # 获取缺陷宏定义
            m = GetContaintofComment(item)
            # 宏定义 缺陷名称
            faultContaint = m.group(3)
            chosenFaults.append(str(m.group(5)))
            faultMap = faultMap + ' ' + m.group(5)
            dstFaultSeeds.write(faultContaint + '\n')
        faultMapFile.write(faultMap + '\n')
        dstFaultSeeds.close()
        comCandidates.remove(comCandidates[comIndex])
        if not IsForbidden(chosenFaults,forbiddenSetList):            
            #  执行脚本进行实验
            os.system('./' + subject + '.gcov.sh')
            #  对比拆分测试用例
            NumFailures = FaultSelection(subject)
            #  保存数据
            if not NumFailures == 0:
                timer = timer + 1
                os.system('mv traces ./' + subject + '/Mul_' + str(n) + '_'+str(numMulVersions))
        else:
            os.system('rm traces -r')
    faultMapFile.close()
    #    print timer

    print len(comn)
    print len(comCandidates)

#========================================================================================================


forbidden1 = []
forbidden2 = []
forbidden3 = [
set(['FAULTY_F_DG_2 ','FAULTY_F_DG_6 '])]
forbidden4 = []
forbidden5 = []

forbiddenList = [forbidden1,forbidden2,forbidden3,forbidden4,forbidden5]

# 生成脚本文件
#os.system('rm v*.sh')
#os.system('./mtscripts.sh')
# 运行oracle
#os.system('./oracle.sh')
# 
subjectList = ['v1','v2','v3','v4','v5']
subjectIndex = 0
for subject in subjectList:
    srcFaultSeeds = file('FaultSeeds.' + subject + '.h','r')
    faultList = []
    for line in srcFaultSeeds:
        m = GetContaintofComment(line)
        if m:
            faultList.append(line)
    # 缺陷总数
    print len(faultList)
    #SingleFaultExperiment(faultList,subject)
    #Mul2FaultExperiment(faultList, subject,forbiddenList[subjectIndex])
    MulnFaultExperiment(faultList, subject, forbiddenList[subjectIndex],3)
    subjectIndex = subjectIndex + 1

haha = []
haha.append('d')
print haha






