#include "pch.h"
#include "MotionDetector.h"
#include "Exceptions.h"
#include <vector>

motion_detector::motion_detector(callback* callback, const unsigned int frame_width, const unsigned int frame_height /* more settings ?? */)
{
	callback_ = callback;
	frame_width_ = frame_width;
	frame_height_ = frame_height;

	// create objects

	// init
	init(); // ?
}

motion_detector::~motion_detector()
{
	// delete used objects and free memory
}

void motion_detector::init()
{
	// init objects here
}

void motion_detector::add_frame(/* frame ?? */)
{
	// process frame
	
	// do stuffs with frame

	// if motion detected use callback to post results
	auto rects = std::vector<RECT>();
	RECT rect1;
	rect1.left = 1; 	rect1.top = 2;	rect1.right = 3;	rect1.bottom = 4;
	rects.push_back(rect1);
	RECT rect2;
	rect2.left = 5; 	rect2.top = 6;	rect2.right = 7;	rect2.bottom = 8;
	rects.push_back(rect2);

	(callback_)(rects.size(), &rects[0], nullptr, 0);
}

void motion_detector::reset()
{
	throw NotImplementedException();
}
