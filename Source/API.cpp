#include "pch.h"
#include "MotionDetector.h"

// create instance and return pointer to instance
MOTION_DETECTION_API h_instance* create_motion_detector(int32_t frame_width, const int32_t frame_height)
{
	// create new instance
	const auto md = new motion_detector(frame_width,  frame_height);
	// return handle
	return md; //reinterpret_cast<h_instance*>(md);
}

// add new frame
MOTION_DETECTION_API int32_t process_frame(h_instance* instance, void* pixels, int32_t bytes_per_line)
{
	auto md = reinterpret_cast<motion_detector*>(instance);

	// create Mat
	auto src_new = Mat(md->frame_height, md->frame_width,CV_8UC3, pixels, bytes_per_line);

	// add new frame
	return md->add_frame(&src_new);
}

MOTION_DETECTION_API void get_motion_regions(h_instance* instance, recta* rects, int32_t rects_count)
{
	auto md = reinterpret_cast<motion_detector*>(instance);

	md->get_regions(rects, rects_count);
}

MOTION_DETECTION_API void get_background_size(h_instance* instance, int32_t width, int32_t height, int32_t bytes_per_pixel, int32_t bytes_per_line)
{
	auto md = reinterpret_cast<motion_detector*>(instance);

	md->get_background_size(width, height, bytes_per_pixel, bytes_per_line);
}

MOTION_DETECTION_API void get_background_image(h_instance* instance, uint8_t* buffer, const int32_t buffer_size)
{
	auto md = reinterpret_cast<motion_detector*>(instance);

	auto bg = md->get_background();

	const auto row_step = bg.step[0];

	if ((buffer_size >= 0) && (buffer_size == unsigned long long(bg.rows * row_step)))
	{
		for (auto i = 0; i < bg.rows; i++) {
			std::memcpy(reinterpret_cast<void*>(buffer[i * row_step]), bg.ptr(i), row_step);
		}
	}
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

	// delete instance
	delete md;
}
