#include <opencv2/opencv.hpp>
#include<iostream>
#include <string>
using namespace cv;
using namespace std;

int main1(int argc, char const *argv[])
{
	VideoCapture cap;
	cap.open("01.mp4");

	if (!cap.isOpened())
		return 0;

	int width = cap.get(CAP_PROP_FRAME_WIDTH);             //帧宽度
	int height = cap.get(CAP_PROP_FRAME_HEIGHT);           //帧高度
	int totalFrames = cap.get(CAP_PROP_FRAME_COUNT);       //总帧数
	int frameRate = cap.get(CAP_PROP_FPS);                 //帧率 x frames/s

	cout << "视频宽度： " << width << endl;
	cout << "视频高度： " << height << endl;
	cout << "视频总帧数： " << totalFrames << endl;
	cout << "帧率： " << frameRate << endl;

	VideoWriter wri;
	int coder = VideoWriter::fourcc('M', 'J', 'P', 'G');//选择编码格式
	wri.open("test.avi", coder, frameRate, Size(width, height));

	Mat frame;
	while (1) {
		cap >> frame;
		if (frame.empty())
			break;
		string sPath = "./picture/0.jpg";
		Mat img = imread(sPath);
		//imshow("Live", img);//显示图像
		wri << img;//把图像写入视频流
		wri << frame;
	}
	cap.release();
	wri.release();

	return 0;
}
