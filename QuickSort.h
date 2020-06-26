#pragma once
#include "Sort.h"
//std::swap
#include <algorithm>
class QuickSort :public Sort{
public:
	// Inherited via Sort
	virtual void sort(int * array, size_t size) override{
		rsort(array, 0, size - 1);
	}
	QuickSort(){}
protected:
	//recursive sort
	void rsort(int* array, int start, int end) {
		int j = split(array, start, end);
		if (start < j)
			rsort(array, start, j - 1);
		if (j + 1 < end)
			rsort(array, j + 1, end);
	}
	int split(int* arr, int start, int end) {
		int i = (start + end) / 2;
		int pivot = arr[i];
		std::swap(arr[i], arr[end]);
		int j = start;
		for (i = start; i < end; i++)
		if (arr[i] < pivot) {
			std::swap(arr[i], arr[j]);
			j++;
		}
		arr[end] = arr[j];
		arr[j] = pivot;
		return j;
	}
	virtual void reset()override{}
};