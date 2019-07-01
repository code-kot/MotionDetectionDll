// Contains declarations of API functions
#pragma once

#ifdef MOTION_DETECTION_EXPORTS
#define MOTION_DETECTION_API __declspec(dllexport)
#else
#define MOTION_DETECTION_API __declspec(dllimport)
#endif

typedef void h_instance;
typedef void (__cdecl callback)(int rects_count, RECT* rects, void* background_pixels, unsigned int bytes_per_line);

extern "C" {
	/// create and init instance
	MOTION_DETECTION_API h_instance* create_motion_detector(callback* callback, unsigned int frame_width, unsigned int frame_height/* settings ?? */);

	/// process next frame
	MOTION_DETECTION_API void process_frame(h_instance* instance, void* pixels, unsigned int bytes_per_line /* frame ?? */);

	/// reset detector
	MOTION_DETECTION_API void reset_motion_detector( h_instance* instance);

	/// delete detector
	MOTION_DETECTION_API void delete_motion_detector(h_instance* instance);
}