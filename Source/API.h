// Contains declarations of API functions
#pragma once
#include "MotionDetector.h"

#ifdef MOTION_DETECTION_EXPORTS
#define MOTION_DETECTION_API __declspec(dllexport)
#else
#define MOTION_DETECTION_API __declspec(dllimport)
#endif

// create and init instance
extern "C" MOTION_DETECTION_API H_instance create_instance(callback callback/* settings ?? */);

// add new frame
extern "C" MOTION_DETECTION_API void add_frame(H_instance instance /* frame ?? */);

// reset
extern "C" MOTION_DETECTION_API void reset_instance(H_instance instance);

// delete
extern "C" MOTION_DETECTION_API void delete_instance(H_instance instance);
