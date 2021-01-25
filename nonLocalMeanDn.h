#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<math.h>
#include<time.h>
#include<vector>
#include"nonLocalMeanDn.h"
using namespace cv;
using namespace std;

struct imageInfo{
	int rows;
	int cols;
	cv::Mat srcImgRgb;
	cv::Mat srcImgGray;
	cv::Mat dstImgGray;
};

struct dnInfo{
	int curWin;//7
	int curWinRadius;//3
	int searchWin;//21
	int serachWinRadius;//10
	float sigma;//16
};

class nonLocalMean{
private: 
	imageInfo imageInfo;
	dnInfo dnInfo;
	void imageShow(cv::Mat image);
	void imageShow(cv::Mat src, cv::Mat dst);
public:
	void initInfo(char *imgPath, int curWin, int searchWin,int sigma);
	void processor();
	void calcPSNR();
};

void nonLocalMean::initInfo(char *imgPath,int curWin,int searchWin,int sigma){
	imageInfo.srcImgRgb = cv::imread(imgPath);
	imageInfo.rows = imageInfo.rows;
	imageInfo.cols = imageInfo.cols;
	cvtColor(imageInfo.srcImgRgb, imageInfo.srcImgGray,COLOR_BGR2GRAY);
	imageInfo.dstImgGray = imageInfo.srcImgGray.clone();
	imageInfo.dstImgGray.convertTo(imageInfo.dstImgGray,CV_32F);
	dnInfo.curWin = curWin;//7
	dnInfo.curWinRadius = curWin >> 1;//3
	dnInfo.searchWin = searchWin;//21
	dnInfo.serachWinRadius = searchWin >> 1;//10
	dnInfo.sigma = sigma;
}

void nonLocalMean::imageShow(cv::Mat image){
	cv::imshow("image",image);
	cv::waitKey(0);
}

void nonLocalMean::imageShow(cv::Mat src,cv::Mat dst){
	cv::imshow("srcImage", src);
	cv::imshow("dstImage", dst);
	cv::waitKey(0);
}

void nonLocalMean::processor(){


	cv::Mat tmp;

	for (int row = dnInfo.serachWinRadius; row < imageInfo.rows - dnInfo.searchWin ; row++)
	{
		for (int col = dnInfo.serachWinRadius; col < imageInfo.cols - dnInfo.searchWin; col++)
		{
			int bRow = row - dnInfo.serachWinRadius;//row - 10
			int bCol = col - dnInfo.serachWinRadius;//col - 10
			//printf("%d,%d,%d,%d\n", row - dnInfo.curWinRadius, col - dnInfo.curWinRadius, dnInfo.curWin, dnInfo.curWin);
			cv::Mat curArea = imageInfo.srcImgGray(cv::Rect(col - dnInfo.curWinRadius, row - dnInfo.curWinRadius, dnInfo.curWin, dnInfo.curWin));
			float vSum = 0.0f;
			float wtSum = 0.0f;
			float tmp1 = 0.0f;
			for (int subRow = bRow; subRow <= (bRow + dnInfo.curWinRadius + dnInfo.serachWinRadius + 1); subRow++)
				for (int subCol = bCol; subCol <= (bCol + dnInfo.curWinRadius + dnInfo.serachWinRadius + 1); subCol++)
				{
					cv::Mat searchArea = imageInfo.srcImgGray(cv::Rect(subCol,subRow, dnInfo.curWin, dnInfo.curWin));
					tmp = searchArea - curArea;
					tmp1 = tmp.dot(tmp);
					tmp1 = sqrt(tmp1);
					tmp1 = exp(-tmp1 / (dnInfo.sigma * dnInfo.sigma));
					wtSum += tmp1;
					vSum += tmp1 * imageInfo.srcImgGray.at<float>(subRow + dnInfo.curWinRadius, subCol + dnInfo.curWinRadius);									   					 				  
				}
			imageInfo.dstImgGray.at<float>(row, col) = vSum / wtSum;
		}		
	}
}

void nonLocalMean::calcPSNR(){

}



void runNonLocalMean(){
	char imgPath[256] = "./data/noiseImage.jpeg";
	nonLocalMean nonLocalMean;
	nonLocalMean.initInfo(imgPath,7,21,16);

}

