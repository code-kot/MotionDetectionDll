#include "pch.h"
//#include "stdafx.h"
#include "MotionDetector.h"

// create instance and return pointer to instance
MOTION_DETECTION_API h_instance* create_motion_detector(callback* callback, const unsigned int frame_width, const unsigned int frame_height/* settings ?? */)
{
	// create new instance
	auto md = new motion_detector(callback, frame_width,  frame_height/* settings ?? */);

	// return handle
	return md; //reinterpret_cast<h_instance*>(md);
}

// add new frame
MOTION_DETECTION_API void process_frame(h_instance* instance, void* pixels, unsigned int bytes_per_line /* frame ?? */)
{
	auto md = reinterpret_cast<motion_detector*>(instance);
	auto src_new = Mat (md->frame_height_, md->frame_width_,CV_8UC3, pixels, bytes_per_line);
	// add new frame
	md->add_frame_(&src_new);
	//auto& src1 = src_new;
	callback(view_Mat);
}

MOTION_DETECTION_API void reset_motion_detector(h_instance* instance)
{
	auto md = reinterpret_cast<motion_detector*>(instance);

	// reset state
	md->reset();
}

MOTION_DETECTION_API void delete_motion_detector(h_instance* instance)
{
	const auto md = reinterpret_cast<motion_detector*>(instance);
	md-> ~motion_detector();
	// delete instance
	delete md;
}
