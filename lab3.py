import sys
import cv2
import matplotlib as mpl
from matplotlib import pyplot as plt
dx=[-1,-1,-1,0,0,0,1,1,1]
dy=[-1,0,1,-1,0,1,-1,0,1] # 周围8个点及自己的相对坐标差
Sobel1=[-1,-2,-1,0,0,0,1,2,1] # Sobel算子对应的矩阵
Sobel2=[-1,0,1,-2,0,2,-1,0,1] # Sobel算子对应的矩阵
Prewitt1=[-1,-1,-1,0,0,0,1,1,1] # Prewitt算子对应的矩阵
Prewitt2=[-1,0,1,-1,0,1,-1,0,1] # Prewitt算子对应的矩阵
Laplace1=[0,1,0,1,-4,1,0,1,0] # Laplace算子对应的矩阵
Laplace2=[-1,-1,-1,-1,8,-1,-1,-1,-1] # Laplace变形算子对应的矩阵

def Roberts(pic):
    (n,m) = pic.shape
    pic1 = [[0 for j in range(m)]for i in range(n)] # 初始化新的灰度图
    for i in range(n):
        for j in range(m):
            if i==n-1 or j==m-1:#边缘上的不作处理
                pic1[i][j]=pic[i][j]
            else:
                pic1[i][j]=abs(int(pic[i][j])-pic[i+1][j+1])+abs(int(pic[i+1][j])-pic[i][j+1])
    return pic1

def calc(pic,mat): # 对pic图片用对应矩阵为mat的算子作处理
    (n,m) = pic.shape
    pic1 = [[0 for j in range(m)]for i in range(n)] # 初始化新的灰度图
    for i in range(n):
        for j in range(m):
            if i==0 or j==0 or i==n-1 or j==m-1:#边缘上的不作处理
                pic1[i][j]=pic[i][j]
            else:
                for k in range(9):
                    pic1[i][j]+=pic[i+dx[k]][j+dy[k]]*mat[k]
    return pic1

def scale(pic,n,m): # 对Laplace变换后的结果作尺度变换将灰度变回0-255范围内
    minv=255
    maxv=0
    for i in range(n): # 统计最大最小的灰度值
        for j in range(m):
            minv=min(minv,pic[i][j])
            maxv=max(maxv,pic[i][j])
    for i in range(n):
        for j in range(m):
            pic[i][j]=round((pic[i][j]-minv)/(maxv-minv)*255)#按比例变换回0-255区间
    return pic
    

path = sys.argv[1] # 要处理的文件的相对路径
pic = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
n,m = pic.shape
pic1 = Roberts(pic)
pic2 = calc(pic,Sobel1)
pic3 = calc(pic,Prewitt1)
pic4 = scale(calc(pic,Laplace1),n,m)
pic5 = scale(calc(pic,Laplace2),n,m)

cmap = mpl.cm.gray
plt.subplot(231),plt.imshow(pic,cmap=cmap), plt.title('origin'),plt.xticks([]), plt.yticks([]) # 显示原图
plt.subplot(232),plt.imshow(pic1,cmap=cmap),plt.title('Roberts'),plt.xticks([]), plt.yticks([]) #显示处理后的图片
plt.subplot(233),plt.imshow(pic2,cmap=cmap),plt.title('Sobel1'),plt.xticks([]), plt.yticks([])
plt.subplot(234),plt.imshow(pic3,cmap=cmap),plt.title('Prewitt1'),plt.xticks([]), plt.yticks([])
plt.subplot(235),plt.imshow(pic4,cmap=cmap),plt.title('Laplace1'),plt.xticks([]), plt.yticks([])
plt.subplot(236),plt.imshow(pic5,cmap=cmap),plt.title('Laplace2'),plt.xticks([]), plt.yticks([])
plt.show()
