#include "QuickSort.h"
#include "PQuickSort.h"
#include <chrono>
#include <thread>
#include <random>
#include <time.h>
#include <array>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace std::chrono;
// if Mode =1 std::array,else raw ptr array 
#define MODE 1
#if MODE==1
constexpr size_t SIZE = 250000;
//mode =1 parallel, mode =0 recursive
void sort(std::array<int, SIZE>& array, int mode, int border=0);
void psort(std::array<int, SIZE>& array, int start, int end,int border);
void rsort(std::array<int, SIZE>&array, int start, int end);
int split(std::array<int, SIZE>&arr, int start, int end);
array<int, SIZE>* generate();
int get_best_border(const array<int, SIZE>& array);
bool compare(std::array<int, SIZE>& original_arr);
//
void sort(std::array<int, SIZE>& array, int mode ,int border) {
	if (mode == 1)
		psort(array, 0, array.size() - 1, border);
	else if(mode==0)
		rsort(array, 0, array.size() - 1);
}
void psort(std::array<int, SIZE>& array, int start, int end,int border) {
	int j = split(array, start, end);
	std::future<void> * t1 = nullptr;
	std::future<void> * t2 = nullptr;
	//border<=0 sorting is full parallel, border>0 bellow border elements sorting is recursive
	if (border <= 0) {
		if (start < j) {
			t1 = new std::future<void>();
			*t1 = std::async(std::launch::async, &psort, std::ref(array), start, j - 1,border);
		}
		if (j + 1 < end) {
			t2 = new std::future<void>();
			*t2 = std::async(std::launch::async, &psort, std::ref(array), j + 1, end, border);
		}
	}
	else {
		if (j - start > border) {
			t1 = new std::future<void>();
			*t1 = std::async(std::launch::async, &psort, std::ref(array), start, j - 1,border);
		}
		else if (j - start > 0)
			rsort(array, start, j - 1);
		if (border < end - (j + 1)) {
			t2 = new std::future<void>();
			*t2 = std::async(std::launch::async, &psort, std::ref(array), j + 1, end,border);
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
void rsort(std::array<int, SIZE>&array, int start, int end) {
	int j = split(array, start, end);
	if (start < j)
		rsort(array, start, j - 1);
	if (j + 1 < end)
		rsort(array, j + 1, end);
}
int split(std::array<int, SIZE>&arr, int start, int end) {
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
array<int, SIZE>* generate() {
	array<int, SIZE>* arr = new array<int, SIZE>();
	random_device seed;
	mt19937 random;
	std::uniform_int_distribution<> range(1, 1000);
	auto entr = seed.entropy();
	if (entr > 0.0)
		random.seed(seed());
	else
		random.seed(time(0));
	for (int i = 0; i < SIZE; i++)
		(*arr)[i] = range(random);
	return arr;
}
int get_best_border(const array<int, SIZE>& array) {
	int min = 500;
	int max = 20000 < SIZE ? 20000 : SIZE;
	cout <<"Max "<< max << endl;
	int border = 0;
	int counter = 0;
	long long t = 20000;
	for (int i = min; i < max; i += 100) {
		std::array<int, SIZE>*temp_arr =new std::array<int,SIZE>;
		*temp_arr = array;
		cout << i << endl;
		auto start = system_clock::now();
		sort(*temp_arr, 1, i);
		auto end = system_clock::now();
		auto time = duration_cast<milliseconds>(end - start).count();
		if (time < t) {
			t = time;
			border = i;
			cout << "New max" << endl;
			counter = 0;
		}
		else {
			counter++;
			if (counter > 12) {
				delete temp_arr;
				cout << "Best time is " << t << " for " << border << " border" << endl;
				return border;
			}
		}
		delete temp_arr;
	}
	cout << "Best time is " << t << " for " << border << " border" << endl;
	return border;
}
bool compare(std::array<int,SIZE>& original_arr) {
	std::array<int, SIZE>*temp_arrqs = new std::array<int, SIZE>;
	std::array<int, SIZE>*temp_arrpqs = new std::array<int, SIZE>;
	*temp_arrpqs = original_arr;
	*temp_arrqs = original_arr;
	sort(*temp_arrpqs, 1);
	sort(*temp_arrqs, 0);
	for (int i = 0; i < SIZE; i++)
		if ((*temp_arrqs)[i] != (*temp_arrpqs)[i]) {
			cout << "Not this same" << endl;
			return false;
		}
	cout << "This same" << endl;
	
	delete temp_arrqs;
	delete temp_arrpqs;
	return true;
}
int main() {
	std::array<int, SIZE> * original_arr = generate();
	get_best_border(*original_arr);
	/*{
		cout << "----SZEREGOWO----" << endl;
		std::array<int, SIZE>* tmpArr = new std::array<int, SIZE>;
		*tmpArr = *original_arr;
		auto t1 = system_clock::now();
		sort(*tmpArr,0);
		auto t2 = system_clock::now();
		cout << "Pomiar 1 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		*tmpArr = *original_arr;
		t1 = system_clock::now();
		sort(*tmpArr, 0);
		t2 = system_clock::now();
		cout << "Pomiar 2 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		*tmpArr = *original_arr;
		t1 = system_clock::now();
		sort(*tmpArr, 0);
		t2 = system_clock::now();
		cout << "Pomiar 3 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		delete tmpArr;
	}*/
	/*{
		cout << "----ROWNOLEGLE----" << endl;
		std::array<int, SIZE>* tmpArr = new std::array<int, SIZE>;
		*tmpArr = *original_arr;
		auto t1 = system_clock::now();
		sort(*tmpArr,1,1000);
		auto t2 = system_clock::now();
		cout << "Pomiar 1 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		*tmpArr = *original_arr;
		t1 = system_clock::now();
		sort(*tmpArr, 1, 2000);
		t2 = system_clock::now();
		cout << "Pomiar 2 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		*tmpArr = *original_arr;
		t1 = system_clock::now();
		sort(*tmpArr, 1, 4000);
		t2 = system_clock::now();
		cout << "Pomiar 3 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		delete tmpArr;
	}*/
	cin.ignore();
	delete original_arr;
	return 0;
}
#else
int * generate(int size) {
	int *arr = new int[size];
	random_device seed;
	mt19937 random;
	std::uniform_int_distribution<> range(1, 1000);
	auto entr = seed.entropy();
	if (entr > 0.0)
		random.seed(seed());
	else
		random.seed(time(0));
	for (int i = 0; i < size; i++)
		arr[i] = range(random);
	return arr;
}
int get_best_border(int *array, int size) {
	int min = 100;
	int max = 20000 < size ? 20000 : size;
	int border = 0;
	long long t = 10000000000000;
	for (int i = min; i < max; i += 100) {
		int * temp_arr = new int[size];
		memcpy(temp_arr, array, size);
		Sort* sort = new PQuickSort(i);
		auto start = system_clock::now();
		sort->sort(temp_arr, size);
		auto end = system_clock::now();
		//auto time = duration_cast<milliseconds>(end - start).count();
		auto time = duration_cast<microseconds>(end - start).count();
		if (time < t) {
			t = time;
			border = i;
		}
		delete sort;
		delete[] temp_arr;
	}
	cout << "Best time is " << t << " for " << border << " border" << endl;
	return border;
}
bool compare(int * original_arr, int size) {
	int * tmpArrqs = new int[size];
	memcpy(tmpArrqs, original_arr, size);
	int * tmpArrpqs = new int[size];
	memcpy(tmpArrpqs, original_arr, size);
	Sort* qsort = new QuickSort();
	qsort->sort(tmpArrqs, size);
	Sort *pqsort = new PQuickSort();
	pqsort->sort(tmpArrpqs, size);
	for (int i = 0; i < size; i++)
		if (tmpArrqs[i] != tmpArrpqs[i]) {
			cout << "Not this same" << endl;
			return false;
		}
	cout << "This same" << endl;
	delete[] tmpArrqs;
	delete[] tmpArrpqs;
	return true;
}
int main() {
	constexpr int SIZE = 1000000;
	int * original_arr = generate(SIZE);
	compare(original_arr, SIZE);
	int bor = get_best_border(original_arr, SIZE);
	{
		cout << "---SZEREGOWO----" << endl;
		int * tmpTab = new int[SIZE];
		memcpy(tmpTab, original_arr, SIZE);
		Sort *sort = new QuickSort();
		auto t1 = system_clock::now();
		sort->sort(tmpTab,SIZE);
		auto t2 = system_clock::now();
		cout << "Pomiar 1 " << duration_cast<milliseconds>(t2-t1).count() << endl;
		memcpy(tmpTab, original_arr, SIZE);
		t1 = system_clock::now();
		sort->sort(tmpTab, SIZE);
		t2 = system_clock::now();
		cout << "Pomiar 2 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		memcpy(tmpTab, original_arr, SIZE);
		t1 = system_clock::now();
		sort->sort(tmpTab, SIZE);
		t2 = system_clock::now();
		cout << "Pomiar 3 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		delete sort;
		delete[]tmpTab;
	}
	{
		cout << "---ROWNOLEGLE----" << endl;
		int * tmpTab = new int[SIZE];
		memcpy(tmpTab, original_arr, SIZE);
		Sort *sort = new PQuickSort();
		auto t1 = system_clock::now();
		sort->sort(tmpTab, SIZE);
		auto t2 = system_clock::now();
		cout << "Pomiar 1 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		memcpy(tmpTab, original_arr, SIZE);
		t1 = system_clock::now();
		sort->sort(tmpTab, SIZE);
		t2 = system_clock::now();
		cout << "Pomiar 2 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		memcpy(tmpTab, original_arr, SIZE);
		t1 = system_clock::now();
		sort->sort(tmpTab, SIZE);
		t2 = system_clock::now();
		cout << "Pomiar 3 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		delete sort;
		delete[]tmpTab;
	}
	cin.ignore();
	delete[]original_arr;
	return 0;
}
#endif // STD==1
