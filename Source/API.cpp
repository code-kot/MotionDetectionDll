#include "pch.h"
#include "MotionDetector.h"
#include <iostream>
#include <cstdint>

//delete instance
void _cdecl release_motion_detector(void* data, int64_t size)
{
	auto _instance_motion_detector = reinterpret_cast<sfc_container*>(data);
	delete[] data;
	delete[] _instance_motion_detector;
}
// create instance and return pointer to instance
MOTION_DETECTION_API sfc_container create_motion_detector()
{
	// create new instance
	auto md = new motion_detector();
	// return handle
	return sfc_container{ md, 1 , &release_motion_detector };
}
//release rectangles from motion_regions
void _cdecl release_motion_regions(void* data, int64_t size)
{
	auto sfc_rect_data = reinterpret_cast<rectangle_struct*>(data);
	delete[] sfc_rect_data;
}

// add new frame
MOTION_DETECTION_API sfc_container process_frame(h_instance* instance, sfc_image_frame* img_frame, int64_t data_count)
{
	auto md = reinterpret_cast<motion_detector*>(instance);
	reinterpret_cast<sfc_image_frame*>(img_frame);
	auto src_new = Mat(img_frame->frame_height, img_frame->frame_width, CV_8UC3,img_frame->pixels, img_frame->bytes_per_line );
	//imshow("src_new", src_new);
	auto add_md = *md->add_frame(src_new); // return vec
	auto sfc_rect = new rectangle_struct[add_md.size()];
	if (!add_md.empty())
	{
		for ( int64_t u = 0; u<add_md.size(); u++)
		{
			sfc_rect[u] = add_md[u]; // vector add_md to structure sfc_rect
		//	cout <<"u:"<<u<<" left:"<<sfc_rect->left<<" top:" << sfc_rect->top<<" height:" << sfc_rect->height<< " width:" << sfc_rect->width << endl;
		}
	}
	return sfc_container{ sfc_rect, static_cast<int64_t>(add_md.size()) , &release_motion_regions };
}

//release background
void _cdecl release_background_image(void* data, int64_t size)
{
	//BYTE background_size = background_image_data->bytes_per_line;
	//BYTE[background_pixels]background_pixels = static_cast<*BYTE[]>(background_image_data->pixels);
	//delete[] background_pixels;
	//auto background_pixels<> = reinterpret_cast<BYTE>(background_image_data->pixels);
	const auto background_image_data = reinterpret_cast<sfc_image_frame*>(data);
	
	delete[] background_image_data;
}

//get background
MOTION_DETECTION_API sfc_container get_background_image(h_instance* instance)
{
	auto md = reinterpret_cast<motion_detector*>(instance);
	auto bg = md->get_background();
	const auto row_step = bg.step[0];
	const auto buffer_bg = new BYTE[bg.rows * row_step];
		for (auto i = 0; i < bg.rows; i++)
		{
			memcpy(reinterpret_cast<void*>(buffer_bg[i * row_step]), bg.ptr(i), row_step);
		}	
	return { new sfc_image_frame{buffer_bg, static_cast<int64_t>(row_step), bg.cols, bg.rows }, 1, &release_background_image };
}

//reset instance
MOTION_DETECTION_API void reset_motion_detector(h_instance* instance)
{
	auto md = reinterpret_cast<motion_detector*>(instance);
	// reset state
	md->reset();
}


