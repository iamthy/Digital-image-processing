import sys
import cv2
import matplotlib as mpl
from matplotlib import pyplot as plt
import random as rd

def add_noise(pic,n,m,rate=0.03): #增加3%的椒盐噪声并输出图片
    pic1 = [[pic[i][j] for j in range(m)] for i in range(n)] # 初始化输出结果
    maxv=0
    minv=255
    for i in range(n):# 记录图像中的灰度最大最小值
        for j in range(m):
            maxv=max(maxv,pic[i][j])
            minv=min(minv,pic[i][j])
    for i in range(n):
        for j in range(m):
            if rd.uniform(0,1)<rate: # 以3%的概率增加噪声
                pic1[i][j] = (minv if rd.uniform(0,1) < 0.5 else maxv)#等概率变为最大或最小的灰度
    return pic1

def mean_filter(pic1,n,m,w=1): # 均值滤波，2*w+1为窗口大小，为奇数
    pic2 = [[pic1[i][j] for j in range(m)] for i in range(n)] # 初始化输出结果
    for i in range(n):
        for j in range(m):
            cnt=0 # 记录在图像范围内的窗口内点个数
            sum=0 # 记录窗口的灰度值的和用来求解均值
            for x in range(i-w,i+w+1):
                if x<0 or x>=n: # 不在范围内的不统计
                    continue
                for y in range(j-w,j+w+1):
                    if y<0 or y>=m:
                        continue
                    cnt+=1
                    sum+=pic1[x][y]
            pic2[i][j]=sum/cnt # 以均值作为结果
    return pic2

def median_filter(pic1,n,m,w=1): # 中值滤波，2*w+1为窗口大小，为奇数
    pic3 = [[pic1[i][j] for j in range(m)] for i in range(n)] # 初始化输出结果
    
    for i in range(n):
        for j in range(m):
            tmp=[] # 用一个临时列表记录窗口内的点的灰度
            for x in range(i-w,i+w+1):
                if x<0 or x>=n: # 不在范围内的不统计
                    continue
                for y in range(j-w,j+w+1):
                    if y<0 or y>=m:
                        continue
                    tmp.append(pic1[x][y])
            cnt=len(tmp) # 记录在图像范围内的窗口内点个数
            tmp.sort()
            if cnt%2 == 1:#取中位数作为处理结果
                pic3[i][j]=tmp[int(cnt/2)]
            else:
                pic3[i][j]=round((int(tmp[int(cnt/2)])+int(tmp[int(cnt/2)-1]))/2)
    return pic3

path = sys.argv[1] # 要处理的文件的相对路径
pic = cv2.imread(path,cv2.IMREAD_GRAYSCALE) # 此次实验读入的是灰度图
n,m=pic.shape
pic1 = add_noise(pic,n,m,0.03) # 对原图增加椒盐噪声
#print(pic1)
pic2 = mean_filter(pic1,n,m,1) # 对有噪声的图片进行均值滤波
pic3 = median_filter(pic1,n,m,1)
cmap = mpl.cm.gray
plt.subplot(221), plt.imshow(pic,cmap=cmap),plt.title('origin'),plt.xticks([]), plt.yticks([]) # 显示原图
plt.subplot(222), plt.imshow(pic1,cmap=cmap),plt.title('noise'),plt.xticks([]), plt.yticks([]) # 显示加了噪声的图片
plt.subplot(223), plt.imshow(pic2,cmap=cmap),plt.title('mean_filter'),plt.xticks([]), plt.yticks([]) # 显示均值滤波的结果
plt.subplot(224), plt.imshow(pic3,cmap=cmap),plt.title('median_filter'),plt.xticks([]), plt.yticks([]) # 显示中值滤波的结果
plt.show()