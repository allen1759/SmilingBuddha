#ifndef MORPHING_H
#define MORPHING_H
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>


using namespace std;
#define PI 3.14159265

//int frame_count = 0;
//IplImage *leftImage, *rightImage;

//int height = 0;
//int width = 0;
//CvScalar Color=CV_RGB(0,255,0);//框框顏色
//int Thickness=2;//框框粗細
//int Shift=0;//框框大小(0為正常)
//int key;//按鍵碼
struct Image{
    int frame_index;//第幾個frame 
	int frame_count;
    IplImage *image;
	IplImage *dest_image;
    IplImage *new_image;
    IplImage *test_image;
	vector<struct LinePair> * pairs;
	vector<struct LinePair> * smile_pairs;
    int width,height;
    Image(int i,int width,int height,IplImage * input,int total,vector<struct LinePair> * pair);//一宣告就初始化 
	Image(int i,int widthh,int heightt,IplImage * input,IplImage * input2,int total,vector<struct LinePair> * pair);
    void LoadImage(string image_name);
    CvScalar bilinear(IplImage *image , double X  , double Y );
    IplImage *  Warp();
	IplImage *  pure_warp();
	double parameter_a;
	double parameter_b;
	double parameter_p;
};
//======================================
struct Line
{
	CvPoint2D32f P; //start 
	CvPoint2D32f Q; //end
	CvPoint2D32f M; //mid
	double len;
	float degree;
	
	
	void PQtoMLD();//已知PQ點 
	void MLDtoPQ();//已知中點,長度,角度 
	//void show();
	
	double Getu(CvPoint2D32f X);
	double Getv(CvPoint2D32f X);
	CvPoint2D32f Get_Point(double u , double v);
	double Get_Weight(CvPoint2D32f X);
};

/*void Line::show()
{
	printf("P(%lf,%lf) Q(%lf,%lf) M(%lf,%lf)\n \tlen=%lf degree=%f\n",P.x,P.y,Q.x,Q.y,M.x,M.y,len,degree);
	return;
}*/

//======================================
struct LinePair
{
	Line leftLine;
	Line rightLine;
	vector<struct Line> warpLine;
	int frame_count;
	void genWarpLine();
	void showWarpLine(int frame_cout);
};

/*
void LinePair::showWarpLine()
{
	int size=warpLine.size();
	for(int i=0;i<size;i++)
	{
		printf("warpLine[%d]:",i);
		warpLine[i].show();
		cvLine(leftImage,cvPointFrom32f(warpLine[i].P),cvPointFrom32f(warpLine[i].Q),Color,Thickness,CV_AA,Shift);
		cvLine(rightImage,cvPointFrom32f(warpLine[i].P),cvPointFrom32f(warpLine[i].Q),Color,Thickness,CV_AA,Shift);
	}
	leftImageTmp = cvCloneImage(leftImage);
	rightImageTmp = cvCloneImage(rightImage);
	cvShowImage("left",leftImage);
	cvShowImage("right",rightImage);
	return;
}*/
//======================================
/*vector<struct LinePair> pairs;
vector<struct LinePair> smile_pairs;
LinePair curLinePair;
LinePair curLinePair_smile;
*/
//======================================
#endif



