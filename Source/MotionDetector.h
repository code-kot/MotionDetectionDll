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

	int32_t background_frames_collected_ = 0;

	Ptr<BackgroundSubtractor> back_sub_;

	static void bh_draw_color_label(Mat& src, const string& title, const Scalar& color, const int pos, const int size = 20); //function create text in window with image

	vector<recta>refine_segments(const Mat& img, Mat& mask, Mat& dst, time_counter& t);	// function find contour from mask of image&find bounding rectangular from contour
	
	void show_images(Mat& img, Mat& mask);

public:
	
	int32_t frame_width;
	int32_t frame_height;
	   	 
	explicit motion_detector( int32_t frame_width, int32_t frame_height /* settings */);
	~motion_detector();
		
	recta rect1;
	int32_t numb_contours;
	vector<recta>rects;
	vector<recta>find_rect;
	Mat view_mat; 
	Mat temp; 
	Mat mask;
	Mat background;
	Mat src1_resized;
	Mat* input_data;
	vector<Mat> src1;
	int32_t j;
	time_counter t; 
	
	void init();
	
	int32_t add_frame(Mat* input_data);

	int32_t rects_f(recta* get_rects, int32_t* number_of_rects);

	Mat& get_background();

	void reset();

	void deinit();
};