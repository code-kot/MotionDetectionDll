#pragma once
#include "API.h"

class motion_detector
{
	callback* callback_;
	unsigned int frame_width_;
	unsigned int frame_height_;

public:
	explicit motion_detector(callback* callback, unsigned int frame_width, unsigned int frame_height /* settings */);

	~motion_detector();

	void init();

	void add_frame(/* frame ?? */);

	void reset();   
};


