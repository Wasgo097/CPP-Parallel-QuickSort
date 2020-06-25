#pragma once
#include<thread>
#include<chrono>
#include <future>
#include <array>
#include "QuickSort.h"
class PQuickSort :public QuickSort{
	int border;
public:
	// Inherited via Sort
	virtual void sort(int * array, size_t size) override{
		psort(array, 0, size - 1);
	}
	PQuickSort(int border = 0) {
		this->border = border;
	}
protected:
	//parallel sort
	void psort(int *array, int start, int end) {
		int j = split(array, start, end);
		std::thread* t1 = nullptr;
		std::thread* t2 = nullptr;
		if (border <= 0) {
			if (start < j) 
				t1 = new std::thread(&PQuickSort::psort, this, array, start, j - 1);
			if (j + 1 < end) 
				t2 = new std::thread(&PQuickSort::psort, this, array, j + 1, end);
		}
		else {
			if (j - start > border)
				t1 = new std::thread(&PQuickSort::psort, this, array, start, j - 1);
			else if (j - start > 0)
				rsort(array, start, j - 1);
			if (border < end - (j + 1))
				t2 = new std::thread(&PQuickSort::psort, this, array, j + 1, end);
			else if (end - (j + 1) > 0)
				rsort(array, j + 1, end);
		}
		if (t1 != nullptr) {
			t1->join();
			delete t1;
		}
		if (t2 != nullptr) {
			t2->join();
			delete t2;
		}
	}
};
