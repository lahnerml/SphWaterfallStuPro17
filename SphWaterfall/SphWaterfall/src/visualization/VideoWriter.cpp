#include <iostream>
#include <string>
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include "VideoWriter.h"
#include "Frame.h"

using namespace std;
using namespace cv;

/*Should work. 
TODO:
You need to add FileStructures for saving the Images : Directory: ""TestPicturePNG" and "TestVideo" and editing height 
+ width in imageWriter 
+ setting Counter in Video Writer (How many Frames you want to add into your video)
*/

void createAlphaMat(Mat &mat, Frame)
{
	CV_Assert(mat.channels() == 4);
	for (int i = 0; i < mat.rows; ++i) {
		for (int j = 0; j < mat.cols; ++j) {

			//Adding Pixels from Frame in BGR (OpenCV-Standart)
			Vec4b& bgra = mat.at<Vec4b>(i, j);
			//to sure about that but should work (maybe change j and i)
			bgra[0] = Frame.getPixel(j,i).at(2); // Blue 255 = UCHAR_MAX
			bgra[1] = Frame.getPixel(j,i).at(1); //green
			bgra[2] = Frame.getPixel(j,i).at(0); //red
			bgra[3] = saturate_cast<uchar>(1 * (bgra[1] + bgra[2])); // Alpha

		}
	}

}

//Saves Images in PNG, imageNumber is the Number of the Frame (important for saving the files in right order)
void imageWriter(Frame , int imageNumber)
{
	//TODO Method for choosing size

	int height = 480;
	int width = 640;
	//save BGR frame Object into Image
	Mat mat(height, width, CV_8UC4);

		createAlphaMat(mat, Frame);
		vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
		compression_params.push_back(0);

		string picturefiles = format("\\TestPicturePNG\\%d.png", imageNumber);
		imwrite(picturefiles, mat, compression_params);
	
}

void videoWriter()
{
	int height;
	int width;
	vector<Mat> images;
	
	//TODO: set Counter (how many Frames do you want to add)
	int Count = 100; 
	for (int i = 0; i<Count; i++)
	{
		//all Images with Names like "1.png", "2.png" ... will add to video, you can change ending for searching for other ImageFiles
		string picturefiles = format("TestPicturePNG\\%d.png", i);
		// dont work : 
		//string picturefiles = format("\\..\\TestPicture\\%d.bmp", a);
		Mat img = imread(picturefiles);
		if (img.empty())      // check resource-loading.
		{
			cerr << picturefiles << i + " can't be loaded!" << endl;
			continue;
		}
		images.push_back(img);

		//cout << picturefiles << endl;
		waitKey();
	}

	//define Size of Video
	Mat size = imread("TestPicturePNG\\1.png");
	height = size.size().height;
	width = size.size().width;
	

	if (images.size() == 0) {
		cerr << "No images available" << endl;
	}
	else { cerr << "images loaded successfully" << endl; }

	VideoWriter outputVideo;
	String outputName = "TestVideo\\outputVideo.avi";
	//TODO: set Frames if you want to and set Size to ImageSize 
	outputVideo.open(outputName, CV_FOURCC('M', 'J', 'P', 'G'), 24, Size(width, height), true);

	if (!outputVideo.isOpened()) {
		cout << "Cannot save Video to file" << endl;

		return -1;
	}

	for (int i = 0; i < images.size(); i++) {
		outputVideo << images.at(i);
	
	}


	cout << "FinishedWriting" << endl;
	return 0;
}