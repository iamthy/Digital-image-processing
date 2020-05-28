import sys
import cv2
import matplotlib as mpl
from matplotlib import pyplot as plt

def linear_trans(pic,k,b): # 对pic图片做线性变换
    (n,m) = pic.shape
    pic1 = [[round(k*pic[i][j]+b) for j in range(m)]for i in range(n)] # 进行线性变换得到新的灰度图
    return pic1

path = sys.argv[1] # 要处理的文件的相对路径
pic = cv2.imread(path,cv2.IMREAD_GRAYSCALE) # 此次实验读入的是灰度图
k = float(input()) # 读入线性变换的斜率和截距
b = float(input())

pic1 = linear_trans(pic,k,b) # 对原图进行线性变换

cmap = mpl.cm.gray

plt.subplot(121),plt.imshow(pic,cmap=cmap),plt.title('origin') # 显示原图
plt.subplot(122), plt.imshow(pic1,cmap=cmap), plt.title(f'tran k={k},b={b}') #显示变换后的图片
plt.show()
'''cv2.namedWindow("Image")
cv2.imshow("Image",pic)
cv2.imshow("Image",pic1)
cv2.waitKey(0)
cv2.destroyAllWindows() '''