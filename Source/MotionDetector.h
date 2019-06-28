#pragma once
#include "API.h"

class motion_detector
{
	callback callback_;

public:
	explicit motion_detector(callback callback /* settings */);

	~motion_detector();

	void init();

	void add_frame(/* frame ?? */);

	void reset();   
};


