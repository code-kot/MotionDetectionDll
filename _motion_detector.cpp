#include <stdexcept>
#include "_motion_detector.h"
#include "time_counter.h"
#include "cv.h"

using namespace std;
using namespace chrono;
using namespace cv;

void motion_detector::bh_draw_color_label(Mat& src, const string& title, const Scalar& color, const int pos,
                                          const int size)
{
	const auto line_size = 30; //30
	const Point_<int> offset(10, 10);
	const auto position = pos * size;
	line(src, Point(offset.x, position + offset.y), Point(offset.x + line_size, position + offset.y),Scalar(20, 55, 220));
	putText(src, title, Point(offset.x + line_size, position + 10), FONT_HERSHEY_COMPLEX, 0.4, Scalar(10, 95, 220));
} //namespace fs = experimental::filesystem;

void motion_detector::refine_segments(const Mat& img, Mat& mask, Mat& dst, time_counter& t)
{
	const auto niters = 3;
	vector<vector<Point>> contours;
	//	vector<Rect> bound_rect(contours.size());
	vector<Vec4i> hierarchy;
	dilate(mask, temp, Mat(), Point(-1, -1), niters);
	erode(temp, temp, Mat(), Point(-1, -1), niters * 2);
	dilate(temp, temp, Mat(), Point(-1, -1), niters);
	findContours(temp, contours, hierarchy, CONTOURS_MATCH_I3, CHAIN_APPROX_SIMPLE); //CHAIN_APPROX_TC89_KCOS
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
	catch (runtime_error& error)
	{
	}
	const Scalar color(255, 255, 255);
	drawContours(dst, contours, largest_comp, color, LINE_AA, LINE_8, hierarchy);

	Point2f vertices[4];

	cvtColor(img, view_mat, CV_8UC1);

	for (const auto& contour : contours)
	{
		const auto roi = boundingRect(contour);
		rectangle(view_mat, roi, Scalar(10, 0, 255));
		//vector<Mat>cropped_image
		auto cropped_image = view_mat(roi);
		imshow("", cropped_image);
		//Mat grayscalie_mat(img.size(), CV_8UC1);
	}
	bh_draw_color_label(view_mat, "Motion Detector", CV_8UC1, (1, 0));
	namedWindow(display_window, WINDOW_AUTOSIZE);
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
