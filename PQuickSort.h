#pragma once
#include<thread>
#include<chrono>
#include<future>
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
		std::future<void> * t1 = nullptr;
		std::future<void> * t2 = nullptr;
		//MyParallelTask t1 = null, t2 = null;
		//border<=0 sorting is full parallel, border>0 bellow border elements sorting is recursive
		if (border <= 0) {
			if (start < j) {
				t1 = new std::future<void>();
				*t1 = std::async(std::launch::async, &PQuickSort::psort, this, array, start, j - 1);
				//t1 = new MyParallelTask(array, start, j - 1);
				//t1.fork();
			}
			if (j + 1 < end) {
				t2 = new std::future<void>();
				*t2 = std::async(std::launch::async, &PQuickSort::psort, this, array, j + 1, end);
				//t2 = new MyParallelTask(array, j + 1, end);
				//t2.fork();
			}
		}
		else {
			if (j - start > border) {
				t1 = new std::future<void>();
				*t1 = std::async(std::launch::async, &PQuickSort::psort, this, array, start, j - 1);
				/*t1 = new MyParallelTask(array, start, j - 1);
				t1.fork();*/
			}
			else if (j - start > 0)
				rsort(array, start, j - 1);
			if (border < end - (j + 1)) {
				t2 = new std::future<void>();
				*t2 = std::async(std::launch::async, &PQuickSort::psort, this, array, j + 1, end);
				/*t2 = new MyParallelTask(array, j + 1, end);
				t2.fork();*/
			}
			else if (end - (j + 1) > 0)
				rsort(array, j + 1, end);
		}
		if (t1 != nullptr) {
			t1->get();
			delete t1;
		}
		if (t2 != nullptr) {
			t2->get();
			delete t2;
		}
	}
};
