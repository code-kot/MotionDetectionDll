#include "pch.h"
#include "API.h"
#include <vector>

std::vector<motion_detector*> g_detectors;
CRITICAL_SECTION g_csDetectorsLock;
bool b_is_detectors_lock_initialized = false;

class lock
{
	CRITICAL_SECTION& cs_;

public:
	explicit lock(CRITICAL_SECTION& cs)
		: cs_(cs)
	{
		EnterCriticalSection(&cs_);
	}
	~lock()
	{
		LeaveCriticalSection(&cs_);
	}
};

void add_detector_instance(motion_detector* md) 
{
	if (!b_is_detectors_lock_initialized)
	{
		b_is_detectors_lock_initialized = true;
		InitializeCriticalSection(&g_csDetectorsLock);
	}

	lock cs_lock(g_csDetectorsLock);
	
	g_detectors.push_back(md);
}

int get_detector_instance_index(motion_detector* md)
{
	auto n = -1;

	if (!b_is_detectors_lock_initialized)
		return n;

	lock cs_lock(g_csDetectorsLock);

	for (auto i = 0; i < g_detectors.size(); i++)
	{
		if (g_detectors[i] == md)
		{
			n = i;
			break;
		}
	}

	return n;
}

bool is_detector_instance_exists(motion_detector* md)
{
	return get_detector_instance_index(md) >= 0;
}

void remove_detector_instance(motion_detector* md)
{
	lock cs_lock(g_csDetectorsLock);

	const auto n = get_detector_instance_index(md);

	if (n >= 0) {
		delete g_detectors[n];
		g_detectors.erase(g_detectors.begin() + n);
	}
}

// create instance and return pointer to instance
MOTION_DETECTION_API H_instance create_instance(const callback callback/* settings ?? */)
{
	// create new instance
	auto md = new motion_detector(callback/* settings ?? */);

	// add to instances list	
	add_detector_instance(md);
	
	// return handle
	return reinterpret_cast<H_instance>(md);
}

// add new frame
MOTION_DETECTION_API void add_frame(const H_instance instance /* frame ?? */)
{
	auto md = reinterpret_cast<motion_detector*>(instance);

	// check instance exists
	if (!is_detector_instance_exists(md))
		return;	// throw exception?

	// add new frame
    md->add_frame(/* frame ?? */);
}

MOTION_DETECTION_API void reset_instance(const H_instance instance)
{
	auto md = reinterpret_cast<motion_detector*>(instance);

	// check instance exists
	if (!is_detector_instance_exists(md))
		return;	// throw exception?

	// reset state
	md->reset();
}

MOTION_DETECTION_API void delete_instance(const H_instance instance)
{
	const auto md = reinterpret_cast<motion_detector*>(instance);

	// delete instance
	remove_detector_instance(md);
}
