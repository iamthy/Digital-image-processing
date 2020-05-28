import sys
import cv2
import matplotlib as mpl
from matplotlib import pyplot as plt

def get_histogram(pic): # 计算图像的直方图,并对图像进行均衡化之后输出
    (n,m) = pic.shape
    pic1 = [[0 for j in range(m)]for i in range(n)] # 初始化新的灰度图
    res = [0 for i in range(256)]
    res2 = [0 for i in range(256)]
    for i in range(n):
        for j in range(m):
            res[pic[i][j]] += 1
    maps = [0 for i in range(256)]
    tot = n * m
    sum = 0 # 计算前缀和
    for i in range(0,256): # 计算灰度均衡化映射
        sum += res[i]
        maps[i] = round(sum/tot*255)
    for i in range(n):#根据均衡化后的灰度映射得出处理后的图片
        for j in range(m):
            pic1[i][j]=maps[pic[i][j]]
            res2[pic1[i][j]] += 1
    return res,res2,pic1

path = sys.argv[1] # 要处理的文件的相对路径
pic = cv2.imread(path,cv2.IMREAD_GRAYSCALE) # 此次实验读入的是灰度图
res,res2,pic1 = get_histogram(pic)
key = [i for i in range(256)]

cmap = mpl.cm.gray
low=int(input())#要显示的直方图区间的上下限
high=int(input())
plt.subplot(221), plt.imshow(pic,cmap=cmap),plt.title('origin'),plt.xticks([]), plt.yticks([]) # 显示原图
plt.subplot(222),plt.imshow(pic1,cmap=cmap),plt.title('equalized'),plt.xticks([]), plt.yticks([]) #显示处理后的图片
plt.subplot(223),plt.xlim([low,high]),  plt.bar(key[low:high],res[low:high]), plt.title('origin')
plt.subplot(224),plt.xlim([0,256]),  plt.bar(key,res2), plt.title('equalized')
plt.show()
