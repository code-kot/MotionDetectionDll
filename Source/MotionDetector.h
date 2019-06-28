#pragma once
#include "Exceptions.h"

typedef unsigned long long H_instance;
typedef void (*callback)(int left, int top /* more params ?? */);

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


