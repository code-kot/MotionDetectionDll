//#include "stdafx.h"
#include "pch.h"
#include "MotionDetector.h"

// create instance and return pointer to instance
MOTION_DETECTION_API h_instance* create_motion_detector(callback* callback, const int frame_width, const int frame_height/* settings ?? */)
{
	// create new instance
	const auto md = new motion_detector(callback, frame_width,  frame_height/* settings ?? */);
	// return handle
	return md; //reinterpret_cast<h_instance*>(md);
}

// add new frame
MOTION_DETECTION_API void process_frame(h_instance* instance, void* pixels, int bytes_per_line /* frame ?? */)
{
	auto md = reinterpret_cast<motion_detector*>(instance);
	auto src_new = Mat (md->frame_height, md->frame_width,CV_8UC3, pixels, bytes_per_line);
	// add new frame
	md->add_frame(&src_new);
	//auto& src1 = src_new;
}

MOTION_DETECTION_API void get_rect(h_instance* instance, void* pixels, int bytes_per_line /* frame ?? */){
	auto md = reinterpret_cast<motion_detector*>(instance);
	auto src_new = Mat(md->frame_height, md->frame_width,CV_8UC3, pixels, bytes_per_line);
	//md->rects_f;
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
