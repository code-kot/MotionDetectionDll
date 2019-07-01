#pragma once

#include <chrono>

using namespace std;
using namespace chrono;

class time_counter {
private: //fields
	chrono::time_point<steady_clock> start_time;
	time_point<steady_clock> end_time;
public: //methods
	double time_span = 0.0;
	void point_start_time();

	void point_end_time();

	void get_time() const;

	void reset_time() {
		time_span = 0.0;
	}
};

