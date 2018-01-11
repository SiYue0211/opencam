#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

//����ͼ���˾���
Mat VintageColor(Mat srcImage)
{
	int width = srcImage.cols;
	int heigh = srcImage.rows;
	Mat dstImage(srcImage.size(), CV_8UC3);
	for (int y = 0; y < heigh; y++)
	{
		uchar* P0 = srcImage.ptr<uchar>(y);
		uchar* P1 = dstImage.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			float B = P0[3 * x];
			float G = P0[3 * x + 1];
			float R = P0[3 * x + 2];
			float newB = 0.272*R + 0.534*G + 0.131*B;
			float newG = 0.349*R + 0.686*G + 0.168*B;
			float newR = 0.393*R + 0.769*G + 0.189*B;
			if (newB < 0)newB = 0;
			if (newB > 255)newB = 255;
			if (newG < 0)newG = 0;
			if (newG > 255)newG = 255;
			if (newR < 0)newR = 0;
			if (newR > 255)newR = 255;
			P1[3 * x] = (uchar)newB;
			P1[3 * x + 1] = (uchar)newG;
			P1[3 * x + 2] = (uchar)newR;
		}

	}
	return dstImage;
}

//���������˾���
Mat ComicStripColor(Mat srcImage)
{
	int width = srcImage.cols;
	int heigh = srcImage.rows;
	RNG rng;
	Mat dstImage(srcImage.size(), CV_8UC3);
	for (int y = 0; y < heigh; y++)
	{
		uchar* P0 = srcImage.ptr<uchar>(y);
		uchar* P1 = dstImage.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			float B = P0[3 * x];
			float G = P0[3 * x + 1];
			float R = P0[3 * x + 2];
			float newB = abs(B - G + B + R)*G / 256;
			float newG = abs(B - G + B + R)*R / 256;
			float newR = abs(G - B + G + R)*R / 256;
			if (newB < 0)newB = 0;
			if (newB > 255)newB = 255;
			if (newG < 0)newG = 0;
			if (newG > 255)newG = 255;
			if (newR < 0)newR = 0;
			if (newR > 255)newR = 255;
			P1[3 * x] = (uchar)newB;
			P1[3 * x + 1] = (uchar)newG;
			P1[3 * x + 2] = (uchar)newR;
		}

	}
	Mat gray;
	cvtColor(dstImage, gray, CV_BGR2GRAY);
	normalize(gray, gray, 255, 0, CV_MINMAX);
	return gray;


}

//�������˾���
Mat Casting(Mat srcImage)
{
	int width = srcImage.cols;
	int height = srcImage.rows;
	Mat dstImage(srcImage.size(), CV_8UC3);
	for (int y = 0; y < height; y++)
	{
		uchar *imgP = srcImage.ptr<uchar>(y);
		uchar *dstP = dstImage.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			float b0 = imgP[3*x];
			float g0 = imgP[3 * x + 1];
			float r0 = imgP[3 * x + 2];

			float b = b0 * 128/(g0 + b0 + 1);
			float g = g0 * 128 / (r0 + b0 + 1);
			float r = r0 * 128 / (b0 + g0 + 1);

			b = (b > 255 ? 255 : (b < 0 ? 0 : b));
			g = (g > 255 ? 255 : (g < 0 ? 0 : g));
			r = (r > 255 ? 255 : (r < 0 ? 0 : r));

			dstP[3 * x] = (uchar)b;
			dstP[3 * x + 1] = (uchar)g;
			dstP[3 * x + 2] = (uchar)r;

		}
	}
	return dstImage;
}

//�����˾���
Mat Emboss(Mat srcImage)
{
	int width = srcImage.cols;
	int height = srcImage.rows;
	Mat dstImage(srcImage.size(), CV_8UC3);
	//���о�������þ���Ϊ[1,0,0;0,0,0;0,0,-1],��һ��,���һ��,��һ�к����һ�в�������
	for (int y = 1; y < height - 1; y++)
	{
		uchar *srcImagePreLine = srcImage.ptr<uchar>(y - 1);
		uchar *srcImageCurrLine = srcImage.ptr<uchar>(y);
		uchar *srcImageNextLine = srcImage.ptr<uchar>(y + 1);
		
		uchar *dstP = dstImage.ptr<uchar>(y);
		for (int x = 1; x < width-1; x++)
		{
			//�ֱ��b,g,r���д���
			for (int i = 0; i < 3; i++)
			{
				int temp = srcImagePreLine[3 * (x - 1) + i] - srcImageNextLine[3 * (x + 1) + i] + 100;
				temp = (temp > 255 ? 255 : (temp < 0 ? 0 : temp));
				dstP[3 * x + i] = temp;
			}
			
		}
	}
	return dstImage;
}

//�������������ţ��˾���
Mat Enlarge(Mat srcImage)
{
	int width = srcImage.cols;
	int heigh = srcImage.rows;
	Point center(width / 2, heigh / 2);
	Mat dstImage(srcImage.size(), CV_8UC3);
	srcImage.copyTo(dstImage);
	srcImage.copyTo(dstImage);

	//��1���Ŵ�  
	int R1 = sqrtf(width*width + heigh*heigh) / 2; //ֱ�ӹ�ϵ���Ŵ������,��R1������;  

	for (int y = 0; y<heigh; y++)
	{
		uchar *img1_p = dstImage.ptr<uchar>(y);
		for (int x = 0; x<width; x++)
		{
			int dis = norm(Point(x, y) - center);
			if (dis<R1)
			{
				int newX = (x - center.x)*dis / R1 + center.x;
				int newY = (y - center.y)*dis / R1 + center.y;

				img1_p[3 * x] = srcImage.at<uchar>(newY, newX * 3);
				img1_p[3 * x + 1] = srcImage.at<uchar>(newY, newX * 3 + 1);
				img1_p[3 * x + 2] = srcImage.at<uchar>(newY, newX * 3 + 2);
			}
		}
	}
	return dstImage;
}

/*
int main()
{
	Mat srcImage = imread("7.jpg");
	Mat dstImage(srcImage.size(), CV_8UC3);
	namedWindow("���ͼ",WINDOW_NORMAL);
	//����ɫ
	//dstImage = VintageColor(srcImage);
	//imshow("����ɫ", dstImage);
	//������
	//dstImage = ComicStripColor(srcImage);
	//imshow("������ɫ", dstImage);
	//����
	dstImage = Casting(srcImage);
	//imshow("����", dstImage);
	//dstImage = Emboss(srcImage);
	//dstImage = Enlarge(srcImage);
	imshow("���ͼ", dstImage);
	waitKey();
}
*/
