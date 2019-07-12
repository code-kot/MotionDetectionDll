#include "pch.h"
#include "MotionDetector.h"
#include "time_counter.h"
#include "cv.h"
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace chrono;
using namespace cv;

void motion_detector::deinit()
{
	src1_resized_.release();
	mask_.release();
	background_.release();
}

void motion_detector::bh_draw_color_label(Mat& src, const string& title, const Scalar& color, const int64_t pos, const int64_t size)
{
	const auto line_size = 30; //30
	const Point_<int64_t> offset(10, 10);
	const auto position = pos * size;
	line(src, Point(offset.x, position + offset.y), Point(offset.x + line_size, position + offset.y), Scalar(20, 55, 220));
	putText(src, title, Point(offset.x + line_size, position + 10), FONT_HERSHEY_COMPLEX, 0.4, Scalar(10, 95, 220));
} //namespace fs = experimental::filesystem;

Ptr<BackgroundSubtractor> back_sub_;

vector<rectangle_struct>* motion_detector::refine_segments(const Mat& img, Mat& mask, Mat& dst, time_counter& t)
{
	const auto niters = 3;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	dilate(mask, temp_, Mat(), Point(-1, -1), niters);
	erode(temp_, temp_, Mat(), Point(-1, -1), niters * 2);
	dilate(temp_, temp_, Mat(), Point(-1, -1), niters);
	findContours(temp_, contours, hierarchy, CONTOURS_MATCH_I3, CHAIN_APPROX_SIMPLE);
	/*
	imshow("temp", temp_);
	waitKey(30);
	*/
	dst = Mat::zeros(img.size(), CV_8UC1);

	if (contours.empty())
	{
		return 0;
	}

	// iterate through all the top-level contours,
	// draw each connected component with its own random color
	auto idx = 0, largest_comp = 0;
	double max_area = 0;
	try
	{
		for (; idx < 0; idx = hierarchy[idx][0])
		{
			const auto& c = contours[idx];
			const auto area = fabs(contourArea(Mat(c)));
			if (area > max_area)
			{
				try
				{
					max_area = area;
					largest_comp = idx;
				}
				catch (runtime_error&)
				{
				}
			}
		}
	}
	catch (runtime_error&)
	{
	}

	const Scalar color(255, 255, 255);
	drawContours(dst, contours, largest_comp, color, LINE_AA, LINE_8, hierarchy);

	cvtColor(img, view_mat_, CV_8UC1);

	auto detected_rects = new vector<rectangle_struct>();

	for (const auto& contour : contours)
	{
		const auto roi = boundingRect(contour);
		detected_rects->push_back(rectangle_struct{ (roi.x) * coef, (roi.y) * coef, (roi.width) * coef, (roi.height) * coef});
	}
	return detected_rects;
}

void motion_detector::show_images(Mat& img, Mat& mask)
{
	imshow("Image", img);
	imshow("Mask", mask);
	imshow("temp", temp_);
	waitKey(1);
	bh_draw_color_label(view_mat_, "Motion Detector", CV_8UC1, (1, 0));
	const auto display_window = "Motion detector";
	namedWindow(display_window, WINDOW_AUTOSIZE);
	imshow(display_window, view_mat_);
}

motion_detector::motion_detector()
{	// init
	init();
}

motion_detector::~motion_detector()
{
	// delete used objects and free memory
	view_mat_.release();
	temp_.release();
	mask_.release();
	background_.release();
	src1_resized_.release();
	back_sub_.release();

	destroyAllWindows();
}

void motion_detector::init()
{	// init objects here
	if (!back_sub_.empty())
		back_sub_.release();

	back_sub_ = createBackgroundSubtractorMOG2(200, 90, true);
	background_frames_collected_ = 0;
	t_.reset_time();
}

vector<rectangle_struct>* motion_detector::add_frame(Mat input_data)
{	
	
	// process frame, do stuffs with frame
	auto &src1 = input_data;
	/*
	imshow("src1", src1);
	waitKey(30);
	*/
	resize(src1, src1_resized_, Size(  src1.cols/coef,  src1.rows/coef));
	
	//imshow("src1_resized",src1_resized_);
	//waitKey(30);
	
	back_sub_->apply(src1_resized_, mask_);
	back_sub_->getBackgroundImage(background_);
	
	//void bh_draw_color_label();
	if (background_frames_collected_ < 20)
	{
		background_frames_collected_++;
	}
	/*
	imshow("src1_res", src1_resized_);
	waitKey(30);
	imshow("mask", mask_);
	waitKey(30);
	imshow("background", background_);
	waitKey(30);
	*/
	return refine_segments(src1_resized_, mask_, temp_, t_);
}

void motion_detector::get_background_size(int64_t width, int64_t height, int64_t bytes_per_pixel, int64_t bytes_per_line)
{
	width = background_.cols;
	height = background_.rows;
	bytes_per_pixel = background_.elemSize();
	bytes_per_line = background_.step[0];
}

Mat& motion_detector::get_background()
{
	return background_;
}

void motion_detector::reset()
{
	back_sub_.reset();
	background_frames_collected_ = 0;
	destroyAllWindows();
}