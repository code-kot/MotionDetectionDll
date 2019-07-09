#include "pch.h"
#include "MotionDetector.h"
#include "time_counter.h"
#include "cv.h"
#include <vector>
#include <stdexcept>

using namespace std;
using namespace chrono;
using namespace cv;

void motion_detector::deinit()
{
	src1_resized_.release();
	mask_.release();
	background_.release();
}

void motion_detector::bh_draw_color_label(Mat& src, const string& title, const Scalar& color, const int32_t pos, const int32_t size)
{
	const auto line_size = 30; //30
	const Point_<int> offset(10, 10);
	const auto position = pos * size;
	line(src, Point(offset.x, position + offset.y), Point(offset.x + line_size, position + offset.y), Scalar(20, 55, 220));
	putText(src, title, Point(offset.x + line_size, position + 10), FONT_HERSHEY_COMPLEX, 0.4, Scalar(10, 95, 220));
} //namespace fs = experimental::filesystem;

void motion_detector::refine_segments(const Mat& img, Mat& mask, Mat& dst, time_counter& t)
{
	const auto niters = 3;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	dilate(mask, temp_, Mat(), Point(-1, -1), niters);
	erode(temp_, temp_, Mat(), Point(-1, -1), niters * 2);
	dilate(temp_, temp_, Mat(), Point(-1, -1), niters);
	findContours(temp_, contours, hierarchy, CONTOURS_MATCH_I3, CHAIN_APPROX_SIMPLE);

	dst = Mat::zeros(img.size(), CV_8UC1);

	if (contours.empty())
		return;

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

	for (const auto& contour : contours)
	{
		const auto roi = boundingRect(contour);
//		rectangle(view_mat_, roi, Scalar(10, 0, 255));

		detected_rects_.push_back(rectangle_struct{ (roi.x) * coef, (roi.y) * coef, (roi.width) * coef, (roi.height) * coef});
	}

//	bh_draw_color_label(view_mat_, "Motion Detector", CV_8UC1, (1, 0));
}

void motion_detector::show_images(Mat& img, Mat& mask)
{
	imshow("Image", img);
	imshow("Mask", mask);
	imshow("temp", temp_);
	bh_draw_color_label(view_mat_, "Motion Detector", CV_8UC1, (1, 0));
	const auto display_window = "Motion detector";
	namedWindow(display_window, WINDOW_AUTOSIZE);
	imshow(display_window, view_mat_);
}

motion_detector::motion_detector(const int frame_width, const int frame_height )
{
	this->frame_width = frame_width;
	this->frame_height = frame_height;

	// create objects

	// init
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

int motion_detector::add_frame(Mat* input_data)
{
	detected_rects_.clear();

	// process frame, do stuffs with frame
	resize(*input_data, src1_resized_, Size(frame_width / coef, frame_height / coef));
	back_sub_->apply(src1_resized_, mask_);
	back_sub_->getBackgroundImage(background_);

	void bh_draw_color_label();
	if (background_frames_collected_ < 20)
	{
		background_frames_collected_++;
		return 0;
	}

	refine_segments(src1_resized_, mask_, temp_, t_);
	return detected_rects_.size();
}

void motion_detector::get_regions(rectangle_struct* rects, const int rects_count)
{
	if (rects_count >= 0 && detected_rects_.size() == uint(rects_count))
	{
		for (auto i = 0; i < rects_count; i++)
			rects[i] = detected_rects_[i];
	}
	else
		throw std::invalid_argument("rects_count does not match detected rects count");
}

void motion_detector::get_background_size(int width, int height, int bytes_per_pixel, int bytes_per_line)
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