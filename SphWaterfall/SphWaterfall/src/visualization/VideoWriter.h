#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>

class video {
public: 
	video();

private:
	vector<Mat> images;
	VideoWriter outputVideo;

};