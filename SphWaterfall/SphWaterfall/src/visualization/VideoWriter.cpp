#include <iostream>
#include <string>
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include "VideoWriter.h"

using namespace std;
using namespace cv;

void video()
{
	//Only works in x64

	vector<Mat> images; //TODO: fill this this with the Images
						//you can use the following line to set Frames by Size
						// Size s = vector[0].getSize(); 

	VideoWriter outputVideo;
	//TODO: set Frames if you want to and set Size to ImageSize 
	outputVideo.open("outputVideo.avi", CV_FOURCC('D', 'I', 'V', 'X'), 24, Size(640, 400), true);

	if (!outputVideo.isOpened()) {
		cout << "Could not open" << endl;
		//return -1;
	}

	for (int i = 0; i < images.size(); i++) {
		outputVideo << images.at(i);
	}

	cout << "FinishedWriting" << endl;
	//return 0;
}