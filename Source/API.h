// Contains declarations of API functions
#pragma once

#ifdef MOTION_DETECTION_EXPORTS
#define MOTION_DETECTION_API __declspec(dllexport)
#else
#define MOTION_DETECTION_API __declspec(dllimport)
#endif

typedef unsigned long long h_instance;
typedef void (*callback)(int left, int top /* more params ?? */);

// create and init instance
extern "C" MOTION_DETECTION_API h_instance create_instance(callback callback/* settings ?? */);

// add new frame
extern "C" MOTION_DETECTION_API void add_frame(h_instance instance /* frame ?? */);

// reset
extern "C" MOTION_DETECTION_API void reset_instance(h_instance instance);

// delete
extern "C" MOTION_DETECTION_API void delete_instance(h_instance instance);
