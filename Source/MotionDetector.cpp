#include "pch.h"
#include "MotionDetector.h"
#include "Exceptions.h"
#include "time_counter.h"
#include "cv.h"
#include <vector>

using namespace std;
using namespace chrono;
using namespace cv;

void motion_detector::deinit()
{
	src1_resized.release();
	mask.release();
	background.release();
}

void motion_detector::bh_draw_color_label(Mat& src, const string& title, const Scalar& color, const int pos, const int size)
{
	const auto line_size = 30; //30
	const Point_<int> offset(10, 10);
	const auto position = pos * size;
	line(src, Point(offset.x, position + offset.y), Point(offset.x + line_size, position + offset.y), Scalar(20, 55, 220));
	putText(src, title, Point(offset.x + line_size, position + 10), FONT_HERSHEY_COMPLEX, 0.4, Scalar(10, 95, 220));
}

vector<recta>motion_detector::refine_segments(const Mat& img, Mat& mask, Mat& dst, time_counter& t)
{
	auto rects = vector<recta>();

	const auto niters = 3;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	dilate(mask, temp, Mat(), Point(-1, -1), niters);
	erode(temp, temp, Mat(), Point(-1, -1), niters * 2);
	dilate(temp, temp, Mat(), Point(-1, -1), niters);
	findContours(temp, contours, hierarchy, CONTOURS_MATCH_I3, CHAIN_APPROX_SIMPLE);
	dst = Mat::zeros(img.size(), CV_8UC1);

	if (contours.empty())
		return rects;
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

	Point2f vertices[4];

	cvtColor(img, view_mat, CV_8UC1);

	recta rect1;
	
	for (const auto& contour : contours)
	{
		Rect roi = boundingRect(contour);
		rectangle(view_mat, roi, Scalar(10, 0, 255));
		rect1.left = (roi.x)*coef; 	rect1.top = (roi.y) * coef;	rect1.right = (roi.width + roi.x)*coef;	rect1.bottom = (roi.height + roi.y)*coef;
		rects.push_back(rect1);
	}
	return rects;
}

void motion_detector::show_images(Mat& img, Mat& mask)
{
	imshow("Image", img);
	imshow("Mask", mask);
	imshow("temp", temp);
	bh_draw_color_label(view_mat, "Motion Detector", CV_8UC1, (1, 0));
	const auto display_window = "Motion detector";
	namedWindow(display_window, WINDOW_AUTOSIZE);
	imshow(display_window, view_mat);
}

motion_detector::motion_detector( const int frame_width, const int frame_height )
{	// create objects
	this->frame_width = frame_width;
	this->frame_height = frame_height;
	// init
	init(); 
}

motion_detector::~motion_detector()
{	// delete used objects and free memory
	back_sub_.release(); 
	src1_resized.release();
	mask.release();
	background.release();
	destroyAllWindows();
}

void motion_detector::init()
{	// init objects here
	if (!back_sub_.empty())
		back_sub_.release();
		back_sub_ = createBackgroundSubtractorMOG2(200, 90, true);
		background_frames_collected_ = 0;
		t.reset_time();
}

int32_t motion_detector::add_frame(Mat* input_data)
{
	find_rect.clear();
	// process frame
	// do stuffs with frame
	auto& src1 = *input_data;
	resize(src1, src1_resized, Size(frame_width / coef, frame_height / coef));
	back_sub_->apply(src1_resized, mask);
	back_sub_->getBackgroundImage(background);
	
	void bh_draw_color_label();
	if (background_frames_collected_ < 1)
	{
		background_frames_collected_++;
		return 0;
	}
	find_rect = refine_segments( src1_resized , mask,  temp,  t);
	int32_t j = find_rect.size();
	return j; 
}

int32_t motion_detector::rects_f(recta* get_rects, int32_t* number_of_rects)
{
	if (j >= *number_of_rects) {
		for (auto x = 0; x < *number_of_rects; x++) {
			get_rects[j] = find_rect[j];
		}
		return 0;
	}
}

Mat& motion_detector::get_background()
{
	return background;
}

void motion_detector::reset()
{
	src1.clear();
	src1_resized.release();
	mask.release();
	background.release();
	destroyAllWindows();
}