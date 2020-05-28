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
const int dx[]={-1,-1,-1,0,0,0,1,1,1}; // 周围8个点及自己的相对坐标差
const int dy[]={-1,0,1,-1,0,1,-1,0,1}; 
int Sobel1[]={-1,-2,-1,0,0,0,1,2,1}; // Sobel算子对应的矩阵
int Sobel2[]={-1,0,1,-2,0,2,-1,0,1}; // Sobel算子对应的矩阵
int Prewitt1[]={-1,-1,-1,0,0,0,1,1,1}; // Prewitt算子对应的矩阵
int Prewitt2[]={-1,0,1,-1,0,1,-1,0,1}; // Prewitt算子对应的矩阵
int Laplace1[]={0,1,0,1,-4,1,0,1,0}; // Laplace算子对应的矩阵
int Laplace2[]={-1,-1,-1,-1,8,-1,-1,-1,-1}; //Laplace变形算子对应的矩阵
char input_path[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/image/blood1.bmp";//输入图像的路径 
char output_path1[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab3/output/Roberts.bmp";//Roberts算子检测结果输出路径 
char output_path2[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab3/output/Sobel1.bmp";//Sobel算子检测结果输出路径 
char output_path3[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab3/output/Prewitt1.bmp";//Prewitt1算子检测结果输出路径 
char output_path4[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab3/output/Laplace1.bmp";//Laplace1算子检测结果输出路径 
char output_path5[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab3/output/Laplace2.bmp";//Laplace2算子检测结果输出路径 
using namespace std;
int w, h, n;
int tmp[1024][1024][4];//临时存放图像的数组
int id(int x,int y,int k){
	return x*w*n+y*n+k;
}

void Roberts(unsigned char * input, unsigned char * output){// 用Roberts算子提取边缘
    int maxv=-1000,minv=1000;//变换后的最大最小值
    for (int s=0;s<n;s++)
		for (int i=0;i<h-1;i++)
			for (int j=0;j<w-1;j++){
                output[id(i,j,s)]=abs(input[id(i,j,s)]-input[id(i+1,j+1,s)])+abs(input[id(i+1,j,s)]-input[id(i,j+1,s)]);
                maxv=max(maxv,(int)output[id(i,j,s)]);
                minv=min(minv,(int)output[id(i,j,s)]);
            }
    for (int s=0;s<n;s++)
		for (int i=1;i<h-1;i++)
			for (int j=1;j<w-1;j++)
                output[id(i,j,s)]=round(1.0*(output[id(i,j,s)]-minv)/(maxv-minv)*255);//按比例把灰度变换回0-255区间
}

void calc(unsigned char * input, unsigned char * output, int mat[]){//其他算子，mat为对应算子的矩阵
    int maxv=-1000,minv=1000;//变换后的最大最小值
    for (int s=0;s<n;s++)
		for (int i=1;i<h-1;i++)
			for (int j=1;j<w-1;j++){
                tmp[i][j][s]=0;
                for (int k=0;k<9;k++){//以邻域中9个点按算子的矩阵加权求和作为结果
                    int x=i+dx[k],y=j+dy[k];
                    tmp[i][j][s]+=mat[k]*(int)input[id(x,y,s)];
                }
                maxv=max(maxv,tmp[i][j][s]);
                minv=min(minv,tmp[i][j][s]);
            }
    for (int s=0;s<n;s++)
		for (int i=1;i<h-1;i++)
			for (int j=1;j<w-1;j++)
                output[id(i,j,s)]=round(255.0*(tmp[i][j][s]-minv)/(maxv-minv));//按比例把灰度变换回0-255区间
}

int main(){
    unsigned char * f = stbi_load(input_path, &w, &h, &n, 0);
    unsigned char * out1 = (unsigned char *) malloc(w*h*n);
    unsigned char * out2 = (unsigned char *) malloc(w*h*n);
    unsigned char * out3 = (unsigned char *) malloc(w*h*n);
    unsigned char * out4 = (unsigned char *) malloc(w*h*n);
    unsigned char * out5 = (unsigned char *) malloc(w*h*n);
	Roberts(f,out1);
    calc(f,out2,Sobel1);
    calc(f,out3,Prewitt1);
    calc(f,out4,Laplace1);
    calc(f,out5,Laplace2);
	stbi_write_bmp(output_path1, w, h, n, out1);
	stbi_write_bmp(output_path2, w, h, n, out2);
	stbi_write_bmp(output_path3, w, h, n, out3);
    stbi_write_bmp(output_path4, w, h, n, out4);
    stbi_write_bmp(output_path5, w, h, n, out5);
    return 0;
}