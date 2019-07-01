#include "pch.h"
#include "time_counter.h"
#include <iostream>

using namespace std;
using namespace chrono;

void time_counter::point_start_time()
{
	start_time = high_resolution_clock::now();
}

void time_counter::point_end_time()
{
	end_time = high_resolution_clock::now();
	time_span += duration_cast<microseconds>(end_time - start_time).count();
}

void time_counter::get_time() const
{
	cout << "WORKING TIME: " << time_span / 1000000 << " seconds." << endl;
}
