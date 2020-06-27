#pragma once
#include <thread>
#include <chrono>
#include <array>
#include <future>
#include <array>
#include <atomic>
#include "QuickSort.h"
class PQuickSort :public QuickSort{
	int threshold;
	std::atomic_int threads;
	const int threds_reset;
public:
	// Inherited via Sort
	virtual void sort(int * array, size_t size) override{
		psort(array, 0, size - 1);
	}
	PQuickSort(int border = 1500, int threads = 12) :threds_reset{ threads } {
		this->threshold = border;
		this->threads = threads;
	}
	//virtual void reset()override {//threads = threds_reset;}
protected:
	//parallel sort
	void psort(int *array, int start, int end) {
		int j = split(array, start, end);
		std::thread* t1 = nullptr;
		std::thread* t2 = nullptr;
		if ((j - start > threshold)&&threads>0) {
			t1 = new std::thread(&PQuickSort::psort, this, array, start, j - 1);
			threads--;
		}
		else if (j - start > 0)
			rsort(array, start, j - 1);
		if ((threshold < end - (j + 1)) && threads>0) {
			t2 = new std::thread(&PQuickSort::psort, this, array, j + 1, end);
			threads--;
		}
		else if (end - (j + 1) > 0)
			rsort(array, j + 1, end);
		if (t1 != nullptr) {
			t1->join();
			threads++;
			delete t1;
		}
		if (t2 != nullptr) {
			t2->join();
			threads++;
			delete t2;
		}
	}
};
