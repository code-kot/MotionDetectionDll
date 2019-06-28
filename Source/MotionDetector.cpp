#include "pch.h"
#include "MotionDetector.h"


motion_detector::motion_detector(callback callback /* more settings ?? */)
{
	callback_ = callback;

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
	(*callback_)(42, 42);
}

void motion_detector::reset()
{
	throw NotImplementedException();
}
