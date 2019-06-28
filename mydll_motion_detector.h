#pragma once
#include <string>
#include<chrono>
#include <opencv2/core.hpp>
#include <filesystem>

#include "time_counter.h"

//typedef unsigned long long H_instance;
//typedef void (*Callback)(int left, int top /* more params ?? */);

using namespace std;
using namespace chrono;
using namespace cv;

class motion_detector
{
private:
	Mat view_mat; Mat temp;
	const string display_window = "Motion detector";

public:
	static void bh_draw_color_label(Mat& src, const string& title, const Scalar& color, const int pos,
	                                const int size = 20) //function create text in window with image
	;

	void refine_segments(const Mat& img, Mat& mask, Mat& dst, time_counter& t)
	// function find contour from mask of image&find bounding rectangular from contour
	;

	void show_images(Mat& img, Mat& mask);

	void reset();
};