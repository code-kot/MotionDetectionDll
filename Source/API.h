// Contains declarations of API functions
#pragma once

//#include "pch.h"
#include <cstdint>

#ifdef MOTION_DETECTION_EXPORTS
#define MOTION_DETECTION_API __declspec(dllexport)
#else
#define MOTION_DETECTION_API __declspec(dllimport)
#endif

typedef void h_instance;

extern "C" {
	/// create and init instance
	MOTION_DETECTION_API h_instance* create_motion_detector(int32_t frame_width, int32_t frame_height);

	/// process next frame
	MOTION_DETECTION_API int32_t process_frame(h_instance* instance, void* pixels, int32_t bytes_per_line);

	/// get regions
	MOTION_DETECTION_API void get_motion_regions(h_instance* instance, RECT* rects, int32_t rects_count);

	/// get background size 
	MOTION_DETECTION_API void get_background_size(h_instance* instance, int32_t width, int32_t height, int32_t bytes_per_pixel, int32_t bytes_per_line);

	/// get background image 
	MOTION_DETECTION_API void get_background_image(h_instance* instance, uint8_t* buffer, int32_t buffer_size);
	
	/// reset detector
	MOTION_DETECTION_API void reset_motion_detector( h_instance* instance);

	/// delete detector
	MOTION_DETECTION_API void delete_motion_detector(h_instance* instance);
}