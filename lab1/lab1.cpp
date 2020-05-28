#include<cmath>
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
using namespace std;
char input_path[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/image/pout.bmp";//输入图像的路径 
char output_path1[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab1/output/linear.bmp";//线性变换输出图像的路径 
char output_path2[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab1/output/stretch.bmp";//拉伸变换输出图像的路径 
char output_path3[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab1/output/histogram.bmp";//图像直方图输出路径 
char output_path4[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab1/output/equalized.bmp";//图像直方图输出路径 

int w, h, n;
int id(int x,int y,int k){
	return x*w*n+y*n+k;
}
int id(int x,int y,int k,int w,int n){
	return x*w*n+y*n+k;
}

void linear_trans(unsigned char * input, unsigned char * output ,double k,double b){ // 线性灰度变换 
	for (int i=0;i<h;i++)
		for (int j=0;j<w;j++)
			for (int s=0;s<n;s++)
				output[id(i,j,s)]=round(1.0*k*input[id(i,j,s)]+b);
}

void stretch_trans(unsigned char * input, unsigned char * output ,double x1,double y1,double x2,double y2){ // 灰度拉伸变换 
	for (int i=0;i<h;i++)
		for (int j=0;j<w;j++)
			for (int s=0;s<n;s++){
				double t = input[id(i,j,s)];
			    if (t < x1)
			        output[id(i,j,s)] = round(y1 / x1 * t);
			    else if (t > x2)
			        output[id(i,j,s)] = round((y2 - y1) / (x2 - x1) * (t - x1) + y1);
			    else
			        output[id(i,j,s)] = round((255 - y2) / (255 - y2) * (t - x2) + y2);
			}
}

void get_histogram(unsigned char * input, unsigned char * histogram, unsigned char * output,int low,int high){
	//直方图均衡，第二个参数是输出的直方图，第三个是输出的均衡后的结果,low和high是统计的灰度范围的上下限 
	int cnt[256],tot=0;//各灰度的出现次数
	memset(cnt,0,sizeof(cnt));
	for (int i=0;i<h;i++)
		for (int j=0;j<w;j++)
			for (int s=0;s<n;s++)
				cnt[input[id(i,j,s)]]++,tot++;
	int maxs=0; //求出出现次数最多的，该灰度对应的直方图高度设为255个像素 
	for (int i=low;i<=high;i++)
		maxs=max(maxs,cnt[i]);
	for (int i=0;i<256;i++)//初始化直方图图像 
		for (int j=0;j<256;j++)
			for (int s=0;s<n;s++)
				histogram[id(i,j,s,256,3)]=0;
	for (int i=0;i<256;i++){//绘制直方图，i为当前绘制的灰度
		int height = round(1.0*cnt[i]/maxs*255);//当前灰度的直方图高度为255*出现次数/最大出现次数
		//printf("%d %d\n",i,height);
		for (int j=0;j<=height;j++)
			histogram[id(255-j,i,0,256,3)]=128;//用红色绘制 
	}
	
	int maps[256],sum=0;//直方图均衡的灰度映射,sum是小于当前灰度的点的个数 
	for (int i=0;i<256;i++){//根据小于该灰度的点的个数除以总个数来得到均衡化后的灰度 
		sum+=cnt[i];
		maps[i]=round(1.0*sum/tot*255);
	}
	for (int i=0;i<h;i++)//根据得到的灰度映射得出处理结果 
		for (int j=0;j<w;j++)
			for (int s=0;s<n;s++)
				output[id(i,j,s)]=maps[input[id(i,j,s)]]; 
}

int main(){
	double k,b,x1,x2,y1,y2;
	scanf("%lf%lf",&k,&b);
	scanf("%lf%lf%lf%lf",&x1,&y1,&x2,&y2);
    unsigned char * f = stbi_load(input_path, &w, &h, &n, 0);
    unsigned char * out1 = (unsigned char *) malloc(w*h*n);
    unsigned char * out2 = (unsigned char *) malloc(w*h*n);
    unsigned char * out3 = (unsigned char *) malloc(w*h*n);
    unsigned char * out4 = (unsigned char *) malloc(w*h*n);
    int low=20,high=200;
    linear_trans(f,out1,k,b);
    stretch_trans(f,out2,x1,y1,x2,y2);
    get_histogram(f,out3,out4,low,high); 
	stbi_write_bmp(output_path1, w, h, n, out1);
	stbi_write_bmp(output_path2, w, h, n, out2);
	stbi_write_bmp(output_path3, 256, 256, n, out3);
	stbi_write_bmp(output_path4, w, h, n, out4);
	stbi_image_free(f);
    return 0;
}
/*
0.5 64
50 50 200 150
*/
