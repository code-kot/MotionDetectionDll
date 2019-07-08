// Contains declarations of API functions
#pragma once

#ifdef MOTION_DETECTION_EXPORTS
#define MOTION_DETECTION_API __declspec(dllexport)
#else
#define MOTION_DETECTION_API __declspec(dllimport)
#endif


typedef void h_instance;
typedef void (__cdecl callback)(int rects_count, RECT* rects, void* background_pixels, int bytes_per_line);

struct recta
{
	int left;
	int top;
	int right;
	int bottom;
};



extern "C" {
	/// create and init instance
	MOTION_DETECTION_API h_instance* create_motion_detector(callback* callback, int frame_width, int frame_height/* settings ?? */);

	/// process next frame
	MOTION_DETECTION_API void process_frame(h_instance* instance, void* pixels, int bytes_per_line /* frame ?? */);

	/// reset detector
	MOTION_DETECTION_API void reset_motion_detector( h_instance* instance);

	/// delete detector
	MOTION_DETECTION_API void delete_motion_detector(h_instance* instance);

	MOTION_DETECTION_API void get_rect(h_instance* instance, void* pixels, int bytes_per_line /* frame ?? */);
}
