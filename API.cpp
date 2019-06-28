#include "pch.h"
#include "API.h"
#include "_motion_detector.h"



using namespace std;
using namespace chrono;
using namespace cv;

std::vector<motion_detector*> g_detectors;
CRITICAL_SECTION g_csDetectorsLock;
bool b_isDetectorsLockInitialized = false;

class lock
{
	CRITICAL_SECTION& cs_;

public:
	lock(CRITICAL_SECTION& cs)
		: cs_(cs)
	{
		EnterCriticalSection(&cs_);
	}
	~lock()
	{
		LeaveCriticalSection(&cs_);
	}
};

void AddDetectorInstance(motion_detector* MD)
{
	if (!b_isDetectorsLockInitialized)
	{
		b_isDetectorsLockInitialized = true;
		InitializeCriticalSection(&g_csDetectorsLock);
	}

	//lock cs_lock(g_csDetectorsLock);

	g_detectors.push_back(MD);
}

int GetDetectorInstanceIndex(motion_detector* MD)
{
	int n = -1;

	if (!b_isDetectorsLockInitialized)
		return n;

	//lock cs_lock(g_csDetectorsLock);

	for (int i = 0; i < g_detectors.size(); i++)
	{
		if (g_detectors[i] == MD)
		{
			n = i;
			break;
		}
	}

	return n;
}

bool IsDetectorInstanceExists(motion_detector* MD)
{
	return GetDetectorInstanceIndex(MD) >= 0;
}

void RemoveDetectorInstance(motion_detector* MD)
{
	//lock cs_lock(g_csDetectorsLock);

	int n = GetDetectorInstanceIndex(MD);

	if (n >= 0) {
		delete g_detectors[n];
		g_detectors.erase(g_detectors.begin() + n);
	}
}

// create instance and return pointer to instance
MOTION_DETECTION_API H_instance create(Callback callback/* settings ?? */)
{
	// create new instance
	motion_detector* MD = new motion_detector(callback/* settings ?? */);

	// add to instances list	
	AddDetectorInstance(MD);

	// return handle
	return (H_instance)MD;
}

// add new frame
MOTION_DETECTION_API void add_frame(H_instance instance /* frame ?? */)
{
	motion_detector* MD = (motion_detector*)instance;

	// check instance exists
	if (!IsDetectorInstanceExists(MD))
		return;	// throw exception?

	// add new frame
	MD->add_frame(/* frame ?? */);

	return;
}

MOTION_DETECTION_API void reset(H_instance instance)
{
	motion_detector* MD = (motion_detector*)instance;

	// check instance exists
	if (!IsDetectorInstanceExists(MD))
		return;	// throw exception?

	// reset state
	MD->reset();

	return;
}

MOTION_DETECTION_API void delete(H_instance instance)
{
	motion_detector* MD = (motion_detector*)instance;

	// delete instance
	RemoveDetectorInstance(MD);

	return;
}