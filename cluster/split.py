import sys
import random

featureSet = open(sys.argv[1],'r')
targetSet = open(sys.argv[2],'r')
outputName = sys.argv[3]
trainSetFeature = open(outputName+'.train.x','w')
testSetFeature = open(outputName+'.test.x','w')
trainSetTarget = open(outputName+'.train.y','w')
testSetTarget = open(outputName+'.test.y','w')
# test FOS
train_size_rate = float(sys.argv[4])#by which rate the dataset is splitted into train/test dataset

featureLine = featureSet.readline()
targetLine = targetSet.readline()
random.seed(99)

while targetLine != '' and featureLine != '':
    print(float(train_size_rate))
    if  random.uniform(0,1.0) < train_size_rate:
        trainSetFeature.write(featureLine)
        trainSetTarget.write(targetLine)
    else :
        testSetFeature.write(featureLine)
        testSetTarget.write(targetLine)
    featureLine = featureSet.readline()
    targetLine = targetSet.readline()