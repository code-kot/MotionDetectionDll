#pragma once
#include "API.h"
#include<chrono>
#include "cv.h"
#include <string>
#include "time_counter.h"

#define coef 6

using namespace std;
using namespace chrono;
using namespace cv;

class motion_detector
{
private:
	const string display_window_ = "Motion detector";

	int background_frames_collected_ = 0;

	Ptr<BackgroundSubtractor> back_sub_;

	static void bh_draw_color_label(Mat& src, const string& title, const Scalar& color, const int pos, const int size = 20); //function create text in window with image

	vector<RECT>refine_segments(const Mat& img, Mat& mask, Mat& dst, time_counter& t);	// function find contour from mask of image&find bounding rectangular from contour

	void show_images(Mat& img, Mat& mask);

	callback* callback_;
public:
	int frame_width;
	int frame_height;

	explicit motion_detector(callback* callback, int frame_width, int frame_height /* settings */);
	~motion_detector();

	Mat view_mat; 
	Mat temp; 
	Mat mask;
	Mat background;
	Mat src1_resized;
	Mat* input_data;
	vector<Mat> src1;

	time_counter t; //create object T

	void init();

	vector<RECT> add_frame(Mat* input_data);

	Mat& get_background();

	void reset();

	void deinit();
};