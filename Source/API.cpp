#include "pch.h"
#include "MotionDetector.h"

// create instance and return pointer to instance
MOTION_DETECTION_API h_instance* create_motion_detector( int32_t frame_width, int32_t frame_height)
{	// create new instance
	auto md = new motion_detector( frame_width,  frame_height); 
	// return handle
	return md; //reinterpret_cast<h_instance*>(md);
}

// add new frame
MOTION_DETECTION_API int32_t process_frame(h_instance* instance, void* pixels, int32_t bytes_per_line )
{
	auto md = reinterpret_cast<motion_detector*>(instance);
	auto src_new = Mat (md->frame_height, md->frame_width,CV_8UC3, pixels, bytes_per_line);
	// add new frame
	md->add_frame(&src_new);
	return 0;
}

MOTION_DETECTION_API int32_t get_rect(h_instance* instance, recta* get_rects, int32_t* number_of_rects )
{
	auto md = reinterpret_cast<motion_detector*>(instance);
	//get rects
	md->rects_f(get_rects, number_of_rects);
	return 0;
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
