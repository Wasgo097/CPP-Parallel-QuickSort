#pragma once
#include <array>
class Sort {
public:
	virtual void sort(int * array, size_t size) = 0;
};
template<int size>
class SortTMP {
public:
	virtual void sort(std::array<int,size>&array) = 0;
};
