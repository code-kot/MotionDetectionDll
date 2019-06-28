// Contains declarations of API functions
#pragma once

#ifdef MOTION_DETECTION_EXPORTS
#define MOTION_DETECTION_API __declspec(dllexport)
#else
#define MOTION_DETECTION_API __declspec(dllimport)
#endif

typedef unsigned long long H_instance;
typedef void (*Callback)(int left, int top /* more params ?? */);

// create and init instance
extern "C" MOTION_DETECTION_API H_instance create(Callback callback/* settings ?? */);

// add new frame
extern "C" MOTION_DETECTION_API void add_frame(H_instance instance /* frame ?? */);

// reset
extern "C" MOTION_DETECTION_API void reset(H_instance instance);

// delete
extern "C" MOTION_DETECTION_API void delete(H_instance instance);
