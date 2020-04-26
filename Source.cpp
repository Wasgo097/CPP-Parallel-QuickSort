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
#define STD 1
#if STD==1
constexpr int SIZE = 1500000;
//template<int size>
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
template<int size>
int get_best_border(const array<int, size>& array) {
	int min = 100;
	int max = 20000 < size ? 20000 : size;
	int border = 0;
	long long t = 10000000000000;
	for (int i = min; i < max; i += 100) {
		std::array<int, size>temp_arr = array;
		Sort* sort = new PQuickSort(i);
		auto start = system_clock::now();
		sort->sort(temp_arr, size);
		auto end = system_clock::now();
		auto time = duration_cast<milliseconds>(end - start).count();
		//auto time = duration_cast<microseconds>(end - start).count();
		if (time < t) {
			t = time;
			border = i;
		}
		delete sort;
	}
	cout << "Best time is " << t << " for " << border << " border" << endl;
	return border;
}
template<int size>
bool compare(std::array<int,size>* original_arr) {
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
	std::array<int, SIZE> * original_arr = generate<SIZE>();
	//{
	//	cout << "---SZEREGOWO----" << endl;
	//	std::array<int, SIZE>* tmpTab = new std::array<int, SIZE>();
	//	*tmpTab = *original_arr;
	//	SortTMP<SIZE> * sort = new QuickSortTMP<SIZE>();
	//	auto t1 = system_clock::now();
	//	sort->sort(*tmpTab);
	//	auto t2 = system_clock::now();
	//	cout << "Pomiar 1 " << duration_cast<milliseconds>(t2-t1).count() << endl;
	//	*tmpTab = *original_arr;
	//	t1 = system_clock::now();
	//	sort->sort(*tmpTab);
	//	t2 = system_clock::now();
	//	cout << "Pomiar 2 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
	//	*tmpTab = *original_arr;
	//	t1 = system_clock::now();
	//	sort->sort(*tmpTab);
	//	t2 = system_clock::now();
	//	cout << "Pomiar 3 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
	//	delete sort;
	//	delete tmpTab;
	//}
	{
		cout << "---ROWNOLEGLE----" << endl;
		std::array<int, SIZE>* tmpArr = new std::array<int, SIZE>();
		*tmpArr = *original_arr;
		//SortTMP<SIZE> * sort = new PQuickSortTMP<SIZE>();
		auto t1 = system_clock::now();
		//sort->sort(*tmpTab);
		PQUickSortTMP::sort<SIZE>(*tmpArr);
		auto t2 = system_clock::now();
		cout << "Pomiar 1 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		*tmpArr = *original_arr;
		t1 = system_clock::now();
		//sort->sort(*tmpTab);
		t2 = system_clock::now();
		cout << "Pomiar 2 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		*tmpArr = *original_arr;
		t1 = system_clock::now();
		//sort->sort(*tmpTab);
		t2 = system_clock::now();
		cout << "Pomiar 3 " << duration_cast<milliseconds>(t2 - t1).count() << endl;
		//delete sort;
		delete tmpArr;
	}
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
