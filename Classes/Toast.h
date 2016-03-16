#pragma once
#include "Global.h"

class Toast
{
public:
	inline long long millisecondNow()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
	void ToastShow(std::string txt, float time = 0.5f, float rotation = 0.0f);
};