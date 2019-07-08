// Contains declarations of API functions
#pragma once

#ifdef MOTION_DETECTION_EXPORTS
#define MOTION_DETECTION_API __declspec(dllexport)
#else
#define MOTION_DETECTION_API __declspec(dllimport)
#endif
#include <cstdint>

typedef void h_instance;

struct recta
{
	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;
};

extern "C" 
{
	// create and init instance
	MOTION_DETECTION_API h_instance* create_motion_detector(int32_t frame_width, int32_t frame_height);
	// process next frame
	MOTION_DETECTION_API int32_t process_frame(h_instance* instance, void* pixels, int32_t bytes_per_line );
	// get rectangles points of coordinates motion detector 
	MOTION_DETECTION_API int32_t get_rect(h_instance* instance, recta* get_rects, int32_t* number_of_rects);
	// reset motion detector
	MOTION_DETECTION_API void reset_motion_detector( h_instance* instance);
	// delete motion detector
	MOTION_DETECTION_API void delete_motion_detector(h_instance* instance);
}
