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
namespace PQUickSortTMP {
	int border=0;
	template<int size>
	void sort(std::array<int, size>& array) {
		PQUickSortTMP::psort(array, 0, array.size() - 1);
	}
	template<int size>
	void psort(std::array<int, size>& array, int start, int end) {
		int j = PQUickSortTMP::split<size>(array, start, end);
		std::future<void> * t1 = nullptr;
		std::future<void> * t2 = nullptr;
		//border<=0 sorting is full parallel, border>0 bellow border elements sorting is recursive
		if (border <= 0) {
			if (start < j) {
				t1 = new std::future<void>();
				t1 = std::async(std::launch::async, &PQUickSortTMP::psort<size>,  array, start, j - 1);
			}
			if (j + 1 < end) {
				t2 = new std::future<void>();
				*t2 = std::async(std::launch::async, &PQUickSortTMP::psort<size>,  array, j + 1, end);
			}
		}
	/*	else {
			if (j - start > border) {
				t1 = new std::future<void>();
				*t1 = std::async(std::launch::async, &psort<size>, this, array, start, j - 1);
			}
			else if (j - start > 0)
				PQUickSortTMP::rsort<size>(array, start, j - 1);
			if (border < end - (j + 1)) {
				t2 = new std::future<void>();
				*t2 = std::async(std::launch::async, &psort<size>, this, array, j + 1, end);
			}
			else if (end - (j + 1) > 0)
				PQUickSortTMP::rsort<size>(array, j + 1, end);
		}*/
		if (t1 != nullptr) {
			t1->get();
			delete t1;
		}
		if (t2 != nullptr) {
			t2->get();
			delete t2;
		}
	}
	template<int size>
	void rsort(std::array<int, size>&array, int start, int end) {
		int j = split(array, start, end);
		if (start < j)
			rsort(array, start, j - 1);
		if (j + 1 < end)
			rsort(array, j + 1, end);
	}
	template<int size>
	int split(std::array<int, size>&arr, int start, int end) {
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
}
//template<int size>
//class PQuickSortTMP :public SortTMP<size> {
//	int border;
//public:
//	PQuickSortTMP(int border = 0){
//		this->border = border; 
//	}
//	virtual void sort(std::array<int, size>& array)override {
//		psort(array, 0, array.size() - 1);
//	}
//protected:
//	void psort(std::array<int, size>& array, int start, int end) {
//		int j = split(array, start, end);
//		std::future<void> * t1 = nullptr;
//		std::future<void> * t2 = nullptr;
//		//border<=0 sorting is full parallel, border>0 bellow border elements sorting is recursive
//		if (border <= 0) {
//			if (start < j) {
//				t1 = new std::future<void>();
//				t1 = std::async(std::launch::async, &PQuickSortTMP<size>::psort, this, array, start, j - 1);
//			}
//			if (j + 1 < end) {
//				t2 = new std::future<void>();
//				*t2 = std::async(std::launch::async, &PQuickSortTMP<size>::psort, this, array, j + 1, end);
//			}
//		}
//		else {
//			if (j - start > border) {
//				t1 = new std::future<void>();
//				*t1 = std::async(std::launch::async, &PQuickSortTMP::psort, this, array, start, j - 1);
//			}
//			else if (j - start > 0)
//				rsort(array, start, j - 1);
//			if (border < end - (j + 1)) {
//				t2 = new std::future<void>();
//				*t2 = std::async(std::launch::async, &PQuickSortTMP::psort, this, array, j + 1, end);
//			}
//			else if (end - (j + 1) > 0)
//				rsort(array, j + 1, end);
//		}
//		if (t1 != nullptr) {
//			t1->get();
//			delete t1;
//		}
//		if (t2 != nullptr) {
//			t2->get();
//			delete t2;
//		}
//	}
//	void rsort(std::array<int, size>&array, int start, int end) {
//		int j = split(array, start, end);
//		if (start < j)
//			rsort(array, start, j - 1);
//		if (j + 1 < end)
//			rsort(array, j + 1, end);
//	}
//	int split(std::array<int, size>&arr, int start, int end) {
//		int i = (start + end) / 2;
//		int pivot = arr[i];
//		std::swap(arr[i], arr[end]);
//		int j = start;
//		for (i = start; i < end; i++)
//			if (arr[i] < pivot) {
//				std::swap(arr[i], arr[j]);
//				j++;
//			}
//		arr[end] = arr[j];
//		arr[j] = pivot;
//		return j;
//	}
//};
