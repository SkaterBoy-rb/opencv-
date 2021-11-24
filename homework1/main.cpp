#include<iostream>
#include<vector>
//#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <highgui.hpp>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include "putText.h"

#define w 1080
using namespace std;
using namespace cv;
VideoWriter writer;
vector<vector<int>> vecx = { {150, 200, 200, 150, 150}, {1050, 1050, 200, 200, 1050}, {1050, 1000, 1000, 1050, 1050},
							{150, 150, 1000, 1000, 150}, {400, 450, 450, 400, 400}, {450, 450, 600, 600, 450},
							{750, 750, 600, 600, 750}, {800, 750, 750, 800, 800} };
vector<vector<int>> vecy = { {50, 50, 900, 900, 50}, {50, 100, 100, 50, 50}, {950, 950, 100, 100, 950}, 
							{950, 900, 900, 950, 950}, {650, 650, 800, 800, 650}, {800, 750, 750, 800, 800},
							{800, 750, 750, 800, 800}, {650, 650, 800, 800, 650} };

// 写入帧
void writeVideo(Mat img, int cnt = 2)
{
	for (int i = 0; i < cnt; i++)
		writer.write(img);
}

// 空格暂停播放，再次点击继续
int KeyBoardRespond(int t = 1000)
{
	int c = waitKey(t);
	if (c == 32)
	{
		c = waitKey(5);
		while (c != 32)
		{
			c = waitKey(5);
		}
	}
	return c;
}

// 画吃豆豆地图
void drawMap(Mat &img)
{
	img.create(w, 1620, CV_8UC3);
	img.setTo(Scalar(0, 0, 0));
	imshow("Live", img);
	for (int j = 0; j < 4; j++)
	{
		int x[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		int y[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		int diff[2] = {0, 0};
		for (int i = 0; i < 8; i++)
		{
			if (vecx[i][j] == vecx[i][j + 1])
			{
				y[i] = (vecy[i][j] < vecy[i][j + 1]) ? 1 : -1;
				diff[i/4] = abs(vecy[i][j] - vecy[i][j + 1]);
			}
			else
			{
				x[i] = (vecx[i][j] < vecx[i][j + 1]) ? 1 : -1;
				diff[i/4] = abs(vecx[i][j] - vecx[i][j + 1]);
			}
		}
		for (int k = 0, t = 0; k < diff[0] || t < diff[1]; k += 3, t += 3)
		{
			for (int i = 0; i < 4; i++)
			{
				circle(img, Point(vecx[i][j] + x[i] * k, vecy[i][j] + y[i] * k), 1, Scalar(255, 50, 200), -1, 8);
				if (t < diff[1])
					circle(img, Point(vecx[i + 4][j] + x[i + 4] * t, vecy[i + 4][j] + y[i + 4] * t), 1, Scalar(255, 100, 100), -1, 8);
			}
			imshow("Live", img);
			writer.write(img);
			KeyBoardRespond(1);
		}
	}
	//填充颜色
	for (int i = 0; i < 8; i++)
		rectangle(img, Point(vecx[i][0], vecy[i][0]), Point(vecx[i][2], vecy[i][2]), Scalar(200, 200, 100), -1);
	//fillPoly(img, contours, Scalar(255, 0, 0));
	imshow("Live", img);
	writer.write(img);
	KeyBoardRespond();
	
	// 画圆
	for (int i = 500; i <= 700; i += 3)
	{
		double x = (double)i;
		circle(img, Point2d(x, 500 + pow(10000 - pow(x - 600, 2), 0.5)), 1, Scalar(200, 200, 100), -1, 8);
		circle(img, Point2d(x, 500 - pow(10000 - pow(x - 600, 2), 0.5)), 1, Scalar(200, 200, 100), -1, 8);
		imshow("Live", img);
		writer.write(img);
		KeyBoardRespond(1);
		
	}
	//  绘制椭圆
	RotatedRect rrt;
	rrt.center = Point2f(400, 350);
	rrt.size = Size(200, 100);
	ellipse(img, rrt, Scalar(200, 200, 100), -1, 8);
	rrt.center = Point2f(800, 350);
	ellipse(img, rrt, Scalar(200, 200, 100), -1, 8);
	imshow("Live", img);
	writer.write(img);
	KeyBoardRespond(10);
	
}

// 绘制豆人 dx dy为朝向
void drawDouDou(Mat &img, int x, int y, int dx, int dy, int del = 0, int radius = 25)
{
	x += 25;
	y += 25;
	if (del == 1)
	{
		circle(img, Point(x, y), radius, Scalar(0, 0, 0), -1, 8);
		return;
	}
	circle(img, Point(x, y), radius, Scalar(0, 255, 255), -1, 8);
	vector<Point> vecPoint;
	vecPoint.push_back(Point(x, y));
	vecPoint.push_back(Point(x + dx * radius + dy * radius/5*3, y + dx * radius / 5 * 3 + dy * radius));
	if (dx == 0)
		vecPoint.push_back(Point(x + dy * -radius / 5 * 3, y + dy * radius));
	else
		vecPoint.push_back(Point(x + dx * radius, y + dx * -radius / 5 * 3));

	fillPoly(img, vecPoint, Scalar(0, 0, 0));
}

// 吃豆豆操作以及字幕
void drawDou(Mat &img)
{
	vector<vector<int>> m(16, vector<int>(14, 0));
	for (int i = 2; i < 6; i++)
	{
		for (int j = 2; j < 4; j++)
		{
			m[i][j] = 1;
			m[i + 8][j] = 1;
		}
	}
	for (int i = 6; i < 10; i++)
		for (int j = 4; j < 8; j++)
			m[i][j] = 1;
	for (int i = 4; i < 12; i++)
	{
		m[i][11] = 1;
		if (i == 4 || i == 11)
		{
			m[i][9] = 1;
			m[i][10] = 1;
		}
	}
	int direct = 3, t = 4, x = 200, y = 200, dx = 1, dy = 0;
	for (int i = 0; i < t; i++)
	{
		int x_rand = rand() % 16;
		int y_rand = rand() % 14;
		if (m[x_rand][y_rand] == 0)
		{
			circle(img, Point(225 + x_rand * 50, 225 + y_rand * 50), 5, Scalar(0, 255, 255), -1, 8);
			m[x_rand][y_rand] = 2;
			continue;
		}
		i--;
	}
	drawDouDou(img, x, y, dx, dy);
	putTextZH(img, "这是一个吃豆豆\n的世界......\n\n(使用WASD操作.)", Point(1100, 100), Scalar(255, 255, 255), 50, "楷体");
	imshow("Live", img);
	writeVideo(img);
	KeyBoardRespond(1000);
	while (t)
	{
		drawDouDou(img, x, y, dx, dy, 1);
		switch (direct)
		{
		case 0: dx = 0; dy = 1; break;
		case 1: dx = 0; dy = -1; break;
		case 2: dx = -1; dy = 0; break;
		case 3: dx = 1; dy = 0; break;
		}
		
 		if (x + dx * 50 >= 200 && x + dx * 50 < 1000 && y + dy * 50 >= 200 && y + dy * 50 < 900 && 
			m[(x + dx * 50 - 200)/50][(y + dy * 50 - 200) / 50] != 1)
		{
			x += dx * 50;
			y += dy * 50;
			if (m[(x - 200) / 50][(y - 200) / 50] == 2)
			{
				t--;
				circle(img, Point(x, y), 5, Scalar(0, 0, 0), -1, 8);
				switch (t)
				{
				case 3: putTextZH(img, "我的生活是去吃豆豆", Point(1100, 300), Scalar(255, 255, 255), 50, "楷体"); break;
				case 1: putTextZH(img, "我也不知道是为了什么，只知道", Point(1100, 500), Scalar(255, 255, 255), 50, "楷体"); break;
				case 0: putTextZH(img, "如果有一天我能吃光...\n\n卧槽！！", Point(1100, 700), Scalar(255, 255, 255), 50, "楷体"); break;
				}
			}
		}
		drawDouDou(img, x, y, dx, dy);
		imshow("Live", img);
		writeVideo(img);
		int c = KeyBoardRespond(500);
		switch (c)
		{
		case 115://下
			direct = 0; break;
		case 119://上 
			direct = 1; break;
		case 97://左
			direct = 2; break;
		case 100://右
			direct = 3; break;
		default: break;
		}
	}

	imshow("Live", img);
	writeVideo(img, 50);
	KeyBoardRespond(2000);
}

//之后的场景绘制
void nextPaint()
{
	Mat img, img1;
	img.create(w, 1620, CV_8UC3);
	img.setTo(Scalar(0, 0, 0));
	rectangle(img, Point(600, 200), Point(1000, 800), Scalar(200, 200, 100));
	rectangle(img, Point(610, 210), Point(990, 790), Scalar(200, 200, 100));
	circle(img, Point(900, 500), 25, Scalar(200, 200, 100), -1, 8);
	line(img, Point(1005, 195), Point(1090, 120), Scalar(255, 255, 255), 4);
	line(img, Point(1005, 205), Point(1090, 170), Scalar(255, 255, 255), 4);
	line(img, Point(1005, 215), Point(1090, 220), Scalar(255, 255, 255), 4);
	putTextZH(img, "叫你吃饭你听不\n\n见是吧,一天天的\n\n就知道睡觉!", 
		Point(1100, 100), Scalar(255, 255, 255), 40, "楷体");
	imshow("Live", img);
	writeVideo(img, 10);
	KeyBoardRespond(2000);
	img1.create(w, 1620, CV_8UC3);
	img1.setTo(Scalar(255, 255, 255));
	putTextZH(img1, "奥......原来是梦......", Point(300, 400), Scalar(0, 0, 0), 100, "楷体");
	for (int j = 20; j <= 2000; j += 20)
	{
		Mat temp;
		addWeighted(img, 1 - (double)j / 2000, img1, (double)j / 2000, 3, temp);
		imshow("Live", temp);
		writer.write(temp);
		KeyBoardRespond(20);
	}
	imshow("Live", img1);
	writeVideo(img);
	KeyBoardRespond(1000);

	//切换
	img1.release();
	img1.create(w, 1620, CV_8UC3);
	img1.setTo(Scalar(0, 0, 0));
	line(img1, Point(1100, 320), Point(1400, 160), Scalar(200, 200, 100), 4);
	line(img1, Point(1100, 320), Point(1100, 740), Scalar(200, 200, 100), 4);
	line(img1, Point(1400, 160), Point(1400, 640), Scalar(200, 200, 100), 4);
	line(img1, Point(1100, 740), Point(1400, 640), Scalar(200, 200, 100), 4);
	line(img1, Point(1115, 335), Point(1385, 175), Scalar(200, 200, 100), 4);
	line(img1, Point(1115, 335), Point(1115, 725), Scalar(200, 200, 100), 4);
	line(img1, Point(1385, 175), Point(1385, 625), Scalar(200, 200, 100), 4);
	line(img1, Point(1115, 725), Point(1385, 625), Scalar(200, 200, 100), 4);
	circle(img1, Point(1165, 450), 15, Scalar(200, 200, 100), -1, 8);
	imshow("Live", img1);
	writeVideo(img1, 10);
	KeyBoardRespond(1000);
	drawDouDou(img1, 900, 560, -1, 0, 0, 100);
	drawDouDou(img1, 300, 460, 1, 0, 0, 200);
	circle(img1, Point(900, 510), 15, Scalar(0, 0, 0), -1, 8);
	circle(img1, Point(300, 360), 35, Scalar(0, 0, 0), -1, 8);
	imshow("Live", img1);
	writeVideo(img1, 10);
	KeyBoardRespond(1000);
	putTextZH(img1, "磨蹭什么呢？赶紧\n\n吃完出去打豆豆", Point(300, 100), Scalar(0, 255, 255), 50, "楷体");
	imshow("Live", img1);
	writeVideo(img1, 50);
	KeyBoardRespond(3000);
	for (int i = 0; i < 6; i++)
	{
		putTextZH(img1, "！", Point(850 + i * 25, 390), Scalar(0, 255, 255), 100, "楷体");
		imshow("Live", img1);
		writeVideo(img1);
		KeyBoardRespond(1000);
	}
	putTextZH(img1, "啊", Point(850, 270), Scalar(0, 255, 255), 100, "楷体");
	imshow("Live", img1);
	writeVideo(img1);
	KeyBoardRespond(400);
	putTextZH(img1, "浙", Point(930, 270), Scalar(0, 255, 255), 100, "楷体");
	imshow("Live", img1);
	writeVideo(img1, 50);
	KeyBoardRespond(3000);
	//切换
	img.release();
	img.create(w, 1620, CV_8UC3);
	img.setTo(Scalar(255, 255, 255));
	putTextZH(img, "欲知后事如何\n\n请听下回分解", Point(500, 400), Scalar(0, 0, 0), 100, "楷体");
	for (int j = 100; j <= 2000; j += 100)
	{
		Mat temp;
		addWeighted(img1, 1 - (double)j / 2000, img, (double)j / 2000, 3, temp);
		imshow("Live", temp);
		writeVideo(temp);
		KeyBoardRespond(100);
	}
	imshow("Live", img);
	writeVideo(img1);
	KeyBoardRespond(2000);
}

// 结尾动画
void RandomLine() {
	Mat img;
	img.create(w, 1620, CV_8UC3);
	img.setTo(Scalar(0, 0, 0));
	imshow("Live", img);
	writer.write(img);
	KeyBoardRespond(100);
	RNG rng(12345);
	Point pt1;
	Point pt2;

	for (int i = 0; i < 10000000; i++) {
		pt1.x = rng.uniform(0, img.cols);
		pt2.x = rng.uniform(0, img.cols);

		pt1.y = rng.uniform(0, img.rows);
		pt2.y = rng.uniform(0, img.rows);

		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));  // 随机产生颜色
		if (waitKey(50) > 0) {
			break;
		}

		line(img, pt1, pt2, color, 4, LINE_8);
		imshow("Live", img);
		writer.write(img);
		KeyBoardRespond(1);

	}
}

// 片头场景+自我介绍
void init()
{
	Mat img;
	int coder = VideoWriter::fourcc('X', 'V', 'I', 'D');//选择编码格式
	double fps = 25;//设置视频帧率
	string sPath = "./picture/1.jpg";
	img = imread(sPath);
	Mat img1;
	Size msize = img.size();
	string filename = "live.avi";
	writer.open(filename, coder, fps, msize, true);//创建保存视频文件的视频流
	if (!writer.isOpened()) {
		cout << "打开视频文件失败，请确认是否为合法输入" << endl;
		return;
	}
	int i = 0;
	while (i < 3) {
		img1 = img;
		string sPath1 = "./picture/" + to_string(i) + ".jpg";
		img = imread(sPath1);
		resize(img, img, msize, 0, 0, INTER_LINEAR);
		putTextZH(img, "学号： 22121236", Point(200, 200), Scalar(0, 255, 255), 60, "楷体");
		putTextZH(img, "任博", Point(280, 350), Scalar(0, 255, 255), 60, "楷体");
		if (i >= 1)
		{
			for (int j = 20; j < 2000; j += 20)
			{
				Mat temp;
				addWeighted(img1, 1 - (double)j / 2000, img, (double)j / 2000, 3, temp);
				imshow("Live", temp);
				writer.write(temp);
				KeyBoardRespond(20);
			}
		}
		imshow("Live", img);
		writeVideo(img);
		KeyBoardRespond(10);
		i++;
	}
}

int main(int argc, char** argv) {
	Mat img;
	init();              // 片头场景+自我介绍
	drawMap(img);        // 绘制吃豆豆地图
	drawDou(img);        // 吃豆豆操作以及字幕
	nextPaint();         // 之后的场景绘制
	RandomLine();        // 结尾动画
	writer.release();
	return 0;
}