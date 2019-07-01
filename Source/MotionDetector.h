#pragma once
#include "API.h"

class motion_detector
{
	callback* callback_;
	int frame_width_;
	int frame_height_;

public:
	explicit motion_detector(callback* callback, int frame_width, int frame_height /* settings */);

	~motion_detector();

	void init();

	void add_frame(/* frame ?? */);

	void reset();   
};


