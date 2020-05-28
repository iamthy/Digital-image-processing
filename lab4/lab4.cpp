#include<cmath>
#include<ctime>
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<algorithm> 
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize.h"
#include "stb_image_write.h"
char input_path[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/image/Rect2.bmp";//输入图像的路径 
char output_path1[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab4/output/Amplitude.bmp";//频谱幅值输出路径 
char output_path2[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab4/output/Phase.bmp";//频谱相位输出路径 
char output_path3[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab4/output/ifftAmplitude.bmp";//由幅度谱逆变换的结果输出路径 
char output_path4[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab4/output/ifftPhase.bmp";//由相位谱逆变换的结果输出路径 
char output_path5[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab4/output/ifft.bmp";//逆变换的结果
using namespace std;
int w, h, n;
int id(int x,int y,int k){
	return x*w*n+y*n+k;
}

struct com{double r,i;}tmp[1024][1024],tmp1[1024];//复数类,临时存放2维FFT的中间结果
com res[1024][1024];
com operator +(com a,com b){return (com){a.r+b.r,a.i+b.i};}
com operator -(com a,com b){return (com){a.r-b.r,a.i-b.i};}
com operator *(com a,com b){return (com){a.r*b.r-a.i*b.i,a.r*b.i+a.i*b.r};}
com operator /(com a,int b){return (com){a.r/b,a.i/b};}

int rev(int x,int len){//求x的二进制表示的逆序表示
    int res=0;
    for (int i=0;i<len;i++){
        res <<= 1;
        if (x&1) res += 1;
        x >>= 1;
    }
    return res;
}

void fft(com *a,int op,int n){//非递归版的FFT，op=1为正变换，op=-1为逆变换
    /*for (int i=0;i<n;i++)
        printf("%d %.3lf %.3lf\n",i,a[i].r,a[i].i);*/
    int len=round(log2(n));
    for (int i=1;i<n;i++) 
        if (rev(i,len)<i) 
            swap(a[rev(i,len)],a[i]);
    for (int sz=2;sz<=n;sz<<=1){//步长
        com w0=(com){cos(2*M_PI/sz),sin(2*M_PI*op/sz)};
        for (int bg=0;bg<n;bg+=sz){
            com w=(com){1,0};
            for (int pos=bg;pos<bg+(sz>>1);pos++){
                com x=a[pos],y=w*a[pos+(sz>>1)];
                a[pos]=x+y,a[pos+(sz>>1)]=x-y;
                w=w*w0;
            }
        }
    }
    /*for (int i=0;i<n;i++)
        printf("%d %.3lf %.3lf\n",i,a[i].r,a[i].i);*/
    if (op==-1) for (int i=0;i<n;i++) a[i]=a[i]/n;
}

void fft_2d(com * input,com * output,int op){//op=1为正变换，op=-1为逆变换
    for (int i=0;i<h;i++){//对每行作FFT
        for (int j=0;j<w;j++)
            tmp1[j]=input[i*w+j];
        fft(tmp1,op,w);
        //if (i==128) for (;;);
        for (int j=0;j<w;j++)
            tmp[i][j]=tmp1[j];
    }
    for (int j=0;j<w;j++){//对每列作FFT
        for (int i=0;i<h;i++)
            tmp1[i]=tmp[i][j];
        fft(tmp1,op,h);
        for (int i=0;i<h;i++)
            output[i*w+j]=tmp1[i];
    }
}

double Ampl(com a){return sqrt(a.r*a.r+a.i*a.i);}//计算复数的模长即频谱的幅度
double logAmpl(com a){return log(Ampl(a)+1);}
double angle(com a){return atan2(a.r,a.i);}//计算相位

com ampli(com a){return (com){Ampl(a),0};}
com phase(com a){return (com){cos(angle(a)),sin(angle(a))};}

void trans(com * res,unsigned char * output,double (*f)(com a)){//把复数结果作f变换（求幅值或相位）到0-255区间作为灰度图像输出
    double maxv=-10000,minv=10000;
    for (int i=0;i<h;i++)
        for (int j=0;j<w;j++){
            maxv=max(maxv,f(res[i*w+j]));
            minv=min(minv,f(res[i*w+j]));
        }
    //printf("%.4lf %.4lf\n",maxv,minv);
    for (int s=0;s<n;s++)
        for (int i=0;i<h;i++)
            for (int j=0;j<=w;j++){
                output[id(i,j,s)]=round(255.0*(f(res[i*w+j])-minv)/(maxv-minv));//按比例变换到0-255区间内
                //if (out1[id(i,j,s)]!=0) printf("%d\n",out1[id(i,j,s)]);
            }
}

void ifft(com * res,unsigned char * output,com (*f)(com a)){//对相位、幅度谱作逆变换
    com * input = (com *) malloc(w*h*sizeof(com));
    com * resA = (com *) malloc(w*h*sizeof(com));
    double maxv=-10000,minv=10000;
    for (int i=0;i<h;i++)
        for (int j=0;j<w;j++){
            input[i*w+j]=f(res[i*w+j]);
        }
    fft_2d(input,resA,-1);//对幅值/相位谱做逆变换
    for (int i=0;i<h;i++)
        for (int j=0;j<w;j++){
            maxv=max(maxv,f(resA[i*w+j]).r);
            minv=min(minv,f(resA[i*w+j]).r);
        }
    for (int s=0;s<n;s++)
        for (int i=0;i<h;i++)
            for (int j=0;j<=w;j++){
                output[id(i,j,s)]=round(255.0*(f(resA[i*w+j]).r-minv)/(maxv-minv));//按比例变换到0-255区间内
            }
}

int main(){
    
    unsigned char * f = stbi_load(input_path, &w, &h, &n, 0);
    /*for (int i=0;i<256;i++)
        printf("%d %d\n",i,rev(i,round(log2(h))));*/
    unsigned char * out1 = (unsigned char *) malloc(w*h*n);
    unsigned char * out2 = (unsigned char *) malloc(w*h*n);
    unsigned char * out3 = (unsigned char *) malloc(w*h*n);
    unsigned char * out4 = (unsigned char *) malloc(w*h*n);
    unsigned char * out5 = (unsigned char *) malloc(w*h*n);
    com * input = (com *) malloc(w*h*sizeof(com));
    com * res = (com *) malloc(w*h*sizeof(com));
    com * mytmp = (com *) malloc(w*h*sizeof(com));
    for (int i=0;i<h;i++)
        for (int j=0;j<w;j++){
            input[i*w+j].r=f[id(i,j,0)];
            input[i*w+j].i=0;
        }

	fft_2d(input,res,1);//正变换
    fft_2d(res,mytmp,-1);
    for (int i=0;i<h;i++)
        for (int j=0;j<w;j++)
            for (int s=0;s<n;s++)
                out5[id(i,j,s)]=round(mytmp[i*w+j].r);//输出直接逆变换的结果
    
    
    trans(res,out2,angle);

    ifft(res,out3,ampli);//对幅值谱逆变换
    ifft(res,out4,phase);//对相位谱作逆变换
    for (int i=0;i<h/2;i++)//把低频移到中心
        for (int j=0;j<w/2;j++)
            swap(res[i*w+j],res[(i+h/2)*w+j+w/2]);
    for (int i=0;i<h/2;i++)
        for (int j=w/2;j<w;j++)
            swap(res[i*w+j],res[(i+h/2)*w+j-w/2]);
    trans(res,out1,logAmpl);//取对数以方便观察

	stbi_write_bmp(output_path1, w, h, n, out1);
	stbi_write_bmp(output_path2, w, h, n, out2);
	stbi_write_bmp(output_path3, w, h, n, out3);
    stbi_write_bmp(output_path4, w, h, n, out4);
    stbi_write_bmp(output_path5, w, h, n, out5);
    return 0;
}
