//
// HW #1-2
// 1-2.cpp
//
// Created by 2014707040 유진혁 Yoo Jinhyeok
//

#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
using namespace std;

// Linear Interpolate Operation
unsigned char LinearInterpolate(unsigned char p1, unsigned char p2, double a)
{
	return (unsigned char)((1 - a) * p1 + a * p2);
}

// Bilinear Interpolate Operation (Interpolates the value of the coordinates x and y of the image.)
unsigned char BilinearInterpolate(vector< vector<char> > &image, double x, double y)
{
	unsigned int pixelX = (unsigned int)x;
	unsigned int pixelY = (unsigned int)y;
	if (pixelX >= 0 && pixelX < image[0].size() && pixelY >= 0 && pixelY < image.size())
	{
		unsigned char p1, p2, p3, p4, value;
		// Not the right-end & the bottom-end pixel
		if ((pixelX < image[0].size() - 1) && (pixelY < image.size() - 1))
		{
			p1 = image[pixelY][pixelX];
			p2 = image[pixelY][pixelX + 1];
			p3 = image[pixelY + 1][pixelX];
			p4 = image[pixelY + 1][pixelX + 1];
			value = LinearInterpolate(LinearInterpolate(p1, p2, x - pixelX), LinearInterpolate(p3, p4, x - pixelX), y - pixelY);
		}
		// The right-end pixel
		else if ((pixelX == image[0].size() - 1) && (pixelY < image.size() - 1))
		{
			p1 = image[pixelY][pixelX];
			p3 = image[pixelY + 1][pixelX];
			value = LinearInterpolate(p1, p3, y - pixelY);
		}
		// The bottom-end pixel
		else if ((pixelX < image[0].size() - 1) && (pixelY == image.size() - 1))
		{
			p1 = image[pixelY][pixelX];
			p2 = image[pixelY][pixelX + 1];
			value = LinearInterpolate(p1, p2, x - pixelX);
		}
		// The right-end & the bottom-end pixel
		else if ((pixelX == image[0].size() - 1) && (pixelY == image.size() - 1))
		{
			value = image[pixelY][pixelX];
		}

		return value;
	}

	return 0;
}

// Rotate a image
string Rotate(string inputFilename, int imgWidth, int imgHeight, double x, string outputFilename = "output.raw")
{
	ifstream fin;
	ofstream fout;
	vector< vector<char> > input(imgHeight, vector<char>(imgWidth));
	vector< vector<char> > output(imgHeight, vector<char>(imgWidth));
	
	// Read a image
	fin.open(inputFilename, ios::binary);
	for (int i = 0; i < imgHeight; i++)
		for (int j = 0; j < imgWidth; j++)
			fin.get(input[i][j]);
	fin.close();

	// Degree to Radian
	double rad = x * M_PI / 180;

	// Write a image
	fout.open(outputFilename, ios::binary);
	for (int i = 0; i < imgHeight; i++)
		for (int j = 0; j < imgWidth; j++)
		{
			// Rotation transformation
			double rotatedX = (cos(rad) * (j - (imgWidth - 1) / 2) + sin(rad) * (i - (imgHeight - 1) / 2)) + (imgWidth - 1) / 2;
			double rotatedY = (-sin(rad) * (j - (imgWidth - 1) / 2) + cos(rad) * (i - (imgHeight - 1) / 2)) + (imgHeight - 1) / 2;
			output[i][j] = BilinearInterpolate(input, rotatedX, rotatedY);
			fout << output[i][j];
		}
	fout.close();

	return outputFilename;
}

int main()
{
	double x;
	cout << "lena256.raw를 회전 시킵니다. 몇 도 회전 시킬까요?" << endl;
	cout << "(30, 45, 60)" << endl;
	cout << ">> ";
	cin >> x;
	cout << "변환 영상이 " << Rotate("lena256.raw", 256, 256, x) << "(으)로 저장되었습니다." << endl;
	return 0;
}