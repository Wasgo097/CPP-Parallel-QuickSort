#pragma once
#include <array>
class Sort {
public:
	virtual void sort(int * array, size_t size) = 0;
	virtual void reset() = 0;
};