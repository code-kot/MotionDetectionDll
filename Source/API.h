// Contains declarations of API functions
#pragma once

#ifdef MOTION_DETECTION_EXPORTS
#define MOTION_DETECTION_API __declspec(dllexport)
#else
#define MOTION_DETECTION_API __declspec(dllimport)
#endif

typedef void h_instance;
typedef void (__cdecl callback)(int left, int top /* more params ?? */);

// create and init instance
extern "C" {
	MOTION_DETECTION_API h_instance* create_instance(callback* callback, unsigned int frame_width, unsigned int frame_height/* settings ?? */);

	// add new frame
	MOTION_DETECTION_API void add_frame(h_instance* instance, void* pixels, unsigned int bytes_per_line /* frame ?? */);

	// reset
	MOTION_DETECTION_API void reset_instance( h_instance* instance);

	// delete
	MOTION_DETECTION_API void delete_instance(h_instance* instance);
}