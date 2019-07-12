// Contains declarations of API functions
#pragma once

//#include "pch.h"
#include <cstdint>
#include "MotionDetector.h"

#ifdef MOTION_DETECTION_EXPORTS
#define MOTION_DETECTION_API __declspec(dllexport)
#else
#define MOTION_DETECTION_API __declspec(dllimport)
#endif

typedef void h_instance;
typedef void(_cdecl release_func)(void* data, int64_t data_count);

struct sfc_container
{
	void* data;
	int64_t data_count;
	release_func* release;
};

struct sfc_image_frame
{//(h_instance* instance, void* pixels, int64_t bytes_per_line, int64_t frame_width, int64_t frame_height)
	void* pixels;
	int64_t bytes_per_line; //src1_resized_.ptr[0]
	int64_t frame_width;
	int64_t frame_height;
};

struct rectangle_struct
{
	int64_t left;
	int64_t top;
	int64_t width;
	int64_t height;
};

extern "C"
{
	//release motion detector
	void _cdecl release_motion_detector(void* data, int64_t size);

	// create and init instance
	MOTION_DETECTION_API sfc_container create_motion_detector();

	//release motion_regions
	void _cdecl release_motion_regions(void* data, int64_t size);

	// process next frame
	MOTION_DETECTION_API sfc_container process_frame(h_instance* instance, sfc_image_frame* img_frame, int64_t data_count); //(h_instance* instance, void* pixels, int64_t bytes_per_line, int64_t frame_width, int64_t frame_height);

	//release background
	void _cdecl release_background_image(void* data, int64_t size);

	// get background image 
	MOTION_DETECTION_API sfc_container get_background_image(h_instance* instance);

	// reset detector
	MOTION_DETECTION_API void reset_motion_detector(h_instance* instance);

	// delete motion detector
	MOTION_DETECTION_API sfc_container delete_motion_detector(void* data, int64_t size);
}
