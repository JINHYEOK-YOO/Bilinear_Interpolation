//
// HW #1-1
// 1-1.cpp
//
// Created by 2014707040 유진혁 Yoo Jinhyeok
//

#include <iostream>
#include <fstream>
#include <vector>
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
		// For not the right-end & the bottom-end pixel
		if ((pixelX < image[0].size() - 1) && (pixelY < image.size() - 1))
		{
			p1 = image[pixelY][pixelX];
			p2 = image[pixelY][pixelX + 1];
			p3 = image[pixelY + 1][pixelX];
			p4 = image[pixelY + 1][pixelX + 1];
			value = LinearInterpolate(LinearInterpolate(p1, p2, x - pixelX), LinearInterpolate(p3, p4, x - pixelX), y - pixelY);
		}
		// For the right-end pixel
		else if ((pixelX == image[0].size() - 1) && (pixelY < image.size() - 1))
		{
			p1 = image[pixelY][pixelX];
			p3 = image[pixelY + 1][pixelX];
			value = LinearInterpolate(p1, p3, y - pixelY);
		}
		// For the bottom-end pixel
		else if ((pixelX < image[0].size() - 1) && (pixelY == image.size() - 1))
		{
			p1 = image[pixelY][pixelX];
			p2 = image[pixelY][pixelX + 1];
			value = LinearInterpolate(p1, p2, x - pixelX);
		}
		// For the right-end & the bottom-end pixel
		else if ((pixelX == image[0].size() - 1) && (pixelY == image.size() - 1))
		{
			value = image[pixelY][pixelX];
		}

		return value;
	}

	return 0;
}

// Enlarge a image
string Scale(string inputFilename, int imgWidth, int imgHeight, double x, string outputFilename = "output.raw")
{
	ifstream fin;
	ofstream fout;
	vector< vector<char> > input(imgHeight, vector<char>(imgWidth));
	int newWidth = (int)(x * imgWidth);
	int newHeight = (int)(x * imgHeight);
	vector< vector<char> > output(newHeight, vector<char>(newWidth));

	// Read a image
	fin.open(inputFilename, ios::binary);
	for (int i = 0; i < imgHeight; i++)
		for (int j = 0; j < imgWidth; j++)
			fin.get(input[i][j]);
	fin.close();

	// Write a image
	fout.open(outputFilename, ios::binary);
	for (int i = 0; i < newHeight; i++)
		for (int j = 0; j < newWidth; j++)
		{
			// Calculate the coordinates when the enlarged image is set to the input image size.
			double interpolatedX = j * (imgWidth - 1) / (newWidth - 1);
			double interpolatedY = i * (imgHeight - 1) / (newHeight - 1);
			output[i][j] = BilinearInterpolate(input, interpolatedX, interpolatedY);
			fout << output[i][j];
		}
	fout.close();

	return outputFilename;
}

int main()
{
	double x;
	cout << "lena256.raw를 확대 시킵니다. 몇 배 확대 시킬까요?" << endl;
	cout << "(436x436 : 1.703125, 512x512 : 2)" << endl;
	cout << ">> ";
	cin >> x;
	cout << "변환 영상이 " << Scale("lena256.raw", 256, 256, x) << "(으)로 저장되었습니다." << endl;
	return 0;
}