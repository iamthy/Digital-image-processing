import sys
import cv2
import matplotlib as mpl
from matplotlib import pyplot as plt

def stretch_trans(pic,x1,y1,x2,y2): # 对pic图片做线性变换
    (n,m) = pic.shape
    pic1 = [[0 for j in range(m)]for i in range(n)] # 初始化新的灰度图
    for i in range(n):
        for j in range(m):
            t = pic[i][j]
            if (t < x1):
                pic1[i][j] = y1 / x1 * t
            elif (t > x2):
                pic1[i][j] = (y2 - y1) / (x2 - x1) * (t - x1) + y1
            else:
                pic1[i][j] = (255 - y2) / (255 - y2) * (t - x2) + y2
    return pic1

path = sys.argv[1] # 要处理的文件的相对路径
pic = cv2.imread(path,cv2.IMREAD_GRAYSCALE) # 此次实验读入的是灰度图
x1 = float(input()) # 读入
y1 = float(input())
x2 = float(input()) 
y2 = float(input())

pic1 = stretch_trans(pic,x1,y1,x2,y2) # 对原图进行线性变换

cmap = mpl.cm.gray

plt.subplot(121), plt.imshow(pic,cmap=cmap),plt.title('origin') # 显示原图
plt.subplot(122), plt.imshow(pic1,cmap=cmap), plt.title(f'tran x1={x1},y1={y1},x2={x2},y2={y2}') #显示变换后的图片
plt.show()
