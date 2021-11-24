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

	int width = cap.get(CAP_PROP_FRAME_WIDTH);             //֡���
	int height = cap.get(CAP_PROP_FRAME_HEIGHT);           //֡�߶�
	int totalFrames = cap.get(CAP_PROP_FRAME_COUNT);       //��֡��
	int frameRate = cap.get(CAP_PROP_FPS);                 //֡�� x frames/s

	cout << "��Ƶ��ȣ� " << width << endl;
	cout << "��Ƶ�߶ȣ� " << height << endl;
	cout << "��Ƶ��֡���� " << totalFrames << endl;
	cout << "֡�ʣ� " << frameRate << endl;

	VideoWriter wri;
	int coder = VideoWriter::fourcc('M', 'J', 'P', 'G');//ѡ������ʽ
	wri.open("test.avi", coder, frameRate, Size(width, height));

	Mat frame;
	while (1) {
		cap >> frame;
		if (frame.empty())
			break;
		string sPath = "./picture/0.jpg";
		Mat img = imread(sPath);
		//imshow("Live", img);//��ʾͼ��
		wri << img;//��ͼ��д����Ƶ��
		wri << frame;
	}
	cap.release();
	wri.release();

	return 0;
}
