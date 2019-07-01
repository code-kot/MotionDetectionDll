#include "pch.h"
#include "MotionDetector.h"
#include "Exceptions.h"

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
	(callback_)(42, 42);
}

void motion_detector::reset()
{
	throw NotImplementedException();
}
