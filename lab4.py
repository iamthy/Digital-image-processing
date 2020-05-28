import sys
import cv2
import matplotlib as mpl
import numpy as npy
from matplotlib import pyplot as plt

def myfft(a,op): # op=1为正变换，-1为逆变换,要求a的长度为2^n
    N=len(a)
    if N==1:#边界处理
        return [a[0]]
    a0 = myfft(a[0:N:2],op) # 下标为偶数的子变换
    a1 = myfft(a[1:N:2],op) # 下标为奇数的子变换
    factor = npy.exp(2j * op * npy.pi / N * npy.arange(N // 2)) # 对应的奇数项要乘的系数w，第i项为w_i即exp(i*2pi/N)
    return npy.append(a0 + factor * a1,a0 - factor * a1)

def call_fft(a):#正变换
    return myfft(a,1)
        

def call_fft2(a):
    return myfft(a,-1)/len(a)

def fft_2d(pic): # 二维FFT
    tmp=npy.apply_along_axis(call_fft,1,npy.asarray(pic)) # 二维的FFT就是对行和列分别作FFT
    return npy.apply_along_axis(call_fft,0,tmp)

def ifft_2d(pic): # 二维逆FFT
    tmp=npy.apply_along_axis(call_fft2,1,npy.asarray(pic)) # 二维的逆FFT就是对行和列分别作逆FFT
    return npy.apply_along_axis(call_fft2,0,tmp)

path = sys.argv[1] # 要处理的文件的相对路径
pic = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
cmap = mpl.cm.gray
res = fft_2d(pic)
A = npy.log(npy.abs(res)+1) # 对频谱的幅度作对数变换
#A = npy.abs(res)+1
sita = npy.angle(res) # 相位

ifftA = npy.real(ifft_2d(npy.abs(res))) # 对幅值作反变换
ifft_sita = npy.real(ifft_2d(sita)) # 对相位作反变换
plt.subplot(221), plt.imshow(pic,cmap=cmap),plt.title('origin'),plt.xticks([]), plt.yticks([]) # 显示原图
plt.subplot(222), plt.imshow(A,cmap=cmap),plt.title('Amplitude'),plt.xticks([]), plt.yticks([]) # 显示对数变换后的幅值
plt.subplot(223), plt.imshow(ifftA,cmap=cmap),plt.title('Amplitude ifft'),plt.xticks([]), plt.yticks([]) # 显示幅值的反变换
plt.subplot(224), plt.imshow(ifft_sita,cmap=cmap),plt.title('Phase ifft'),plt.xticks([]), plt.yticks([]) # 显示幅值的反变换
plt.show()