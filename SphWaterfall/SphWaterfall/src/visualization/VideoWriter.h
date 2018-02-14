#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>

using namespace std;

class VideoWriter {
public: 
	void videoWriter();
	void imageWriter(Frame);
	void createAlphaMat(Mat &mat, int pixel[3]);

private:
	vector<Mat> images;
	VideoWriter outputVideo;
	int height;
	int width;
	int Count;
	int pixel[3];
	string picturefiles;
	Mat img;
};