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

	void refine_segments(const Mat& img, Mat& mask, Mat& dst, time_counter& t);	// function find contour from mask of image&find bounding rectangular from contour

	void show_images(Mat& img, Mat& mask);

	vector<rectangle_struct> detected_rects_;

	Mat view_mat_;
	Mat temp_;
	Mat mask_;
	Mat background_;
	Mat src1_resized_;

	time_counter t_;
public:
	int frame_width;
	int frame_height;

	explicit motion_detector(int frame_width, int frame_height);
	~motion_detector();

	void init();

	int add_frame(Mat* input_data);

	void get_regions(rectangle_struct* rects, int rects_count);

	void get_background_size(int width, int height, int bytes_per_pixel, int bytes_per_line);

	Mat& get_background();

	void reset();

	void deinit();
};