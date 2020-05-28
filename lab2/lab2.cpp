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
using namespace std;
char input_path[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/image/lena.bmp";//输入图像的路径 
char output_path1[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab2/output/noise.bmp";//增加噪声后的图像输出路径 
char output_path2[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab2/output/mean_filter.bmp";//均值滤波后的图像输出路径 
char output_path3[100] = "E:/STUDY/Computer Graphics/Digital-image-processing/lab2/output/median_filter.bmp";//中值滤波后的图像输出路径 
int w, h, n;
int id(int x,int y,int k){
	return x*w*n+y*n+k;
}

void add_noise(unsigned char * input, unsigned char * output,double rate=0.03){//增加3%椒盐噪声
	int minv=255,maxv=0;
	for (int i=0;i<h;i++)
		for (int j=0;j<h;j++)
			for (int s=0;s<n;s++){
				maxv=max(maxv,(int)input[id(i,j,s)]);
				minv=min(minv,(int)input[id(i,j,s)]);
			}
	for (int i=0;i<h;i++)
		for (int j=0;j<h;j++)
			if (1.0*rand()/RAND_MAX<rate){
				int noise = 1.0*rand()/RAND_MAX<0.5?maxv:minv;
				for (int s=0;s<n;s++)
					output[id(i,j,s)]=noise;
			}
			else{
				for (int s=0;s<n;s++)
					output[id(i,j,s)]=input[id(i,j,s)];
			}
}

void mean_filter(unsigned char * input, unsigned char * output,int siz=1){//均值滤波，2*siz+1为窗口边长，为奇数
	for (int s=0;s<n;s++)
		for (int i=0;i<h;i++)
			for (int j=0;j<w;j++){
				int cnt=0,sum=0;//cnt是在图像有效范围内的且在当前点邻域中的点的个数，sum是邻域有效点的灰度之和
				for (int x=i-siz;x<=i+siz;x++)
					for (int y=j-siz;y<=j+siz;y++){
						if (x<0||x>=h||y<0||y>=w) continue;
						cnt++,sum+=input[id(x,y,s)];
					}
				output[id(i,j,s)]=round(1.0*sum/cnt);//以邻域灰度均值作为平均
			}
}

void median_filter(unsigned char * input, unsigned char * output,int siz=1){//中值滤波，2*siz+1为窗口边长，为奇数
	for (int s=0;s<n;s++)
		for (int i=0;i<h;i++)
			for (int j=0;j<w;j++){
				int cnt=0;//cnt是在图像有效范围内的且在当前点邻域中的点的个数
				int tmp[101];
				for (int x=i-siz;x<=i+siz;x++)
					for (int y=j-siz;y<=j+siz;y++){
						if (x<0||x>=h||y<0||y>=w) continue;
						tmp[cnt++]=input[id(x,y,s)];
					}
				sort(tmp,tmp+cnt);
				if (cnt%2==0)//取邻域灰度的中位数作为滤波结果
					output[id(i,j,s)]=(tmp[cnt/2]+tmp[cnt/2-1])/2;//有两个中位数就取平均
				else output[id(i,j,s)]=tmp[cnt/2];
			}
}

int main(){
	srand(time(0));
	unsigned char * f = stbi_load(input_path, &w, &h, &n, 0);
    unsigned char * out1 = (unsigned char *) malloc(w*h*n);
    unsigned char * out2 = (unsigned char *) malloc(w*h*n);
    unsigned char * out3 = (unsigned char *) malloc(w*h*n);
	add_noise(f,out1,0.03);
	mean_filter(out1,out2,1);
	median_filter(out1,out3,1);
	stbi_write_bmp(output_path1, w, h, n, out1);
	stbi_write_bmp(output_path2, w, h, n, out2);
	stbi_write_bmp(output_path3, w, h, n, out3);
	return 0;
}

