#include "QuickSort.h"
#include "PQuickSort.h"
#include <chrono>
#include <thread>
#include <random>
#include <ctime>
#include <iostream>
using namespace std;
using namespace std::chrono;
int * generate(int size) {
	int *arr = new int[size];
	random_device seed;
	mt19937 random;
	std::uniform_int_distribution<> range(1, 1000);
	auto entr = seed.entropy();
	if (entr > 0.0)
		random.seed(seed());
	else
		random.seed(time(0));	for (int i = 0; i < size; i++)
		arr[i] = range(random);
	return arr;
}
int get_best_border(int *array, int size) {
	int min = 100;
	int max = 20000 < size ? 20000 : size;
	int border = 0;
	long long t = 10000000000000;
	for (int i = min; i < max; i+=100) {
		int * temp_arr = new int[size];
		memcpy(temp_arr, array, size);
		Sort* sort = new PQuickSort(i);
		auto start = system_clock::now();
		sort->sort(temp_arr, size);
		auto end = system_clock::now();
		auto time = duration_cast<milliseconds>(end - start).count();
		if (time < t) {
			t = time;
			border = i;
		}
		delete sort;
		delete[] temp_arr;
	}
	cout << "Best time is " << t << " for " << border << " border"<<endl;
	return border;
}
bool compare(int * original_arr, int size) {
	int * tmpArrqs = new int[size];
	memcpy(tmpArrqs, original_arr, size);
	int * tmpArrpqs = new int[size];
	memcpy(tmpArrpqs, original_arr, size);
	//cout << "Stworzono tablice" << endl;
	Sort* qsort = new QuickSort();
	qsort->sort(tmpArrqs,size);
	//cout << "Sort szeregowy" << endl;
	Sort *pqsort = new PQuickSort();
	pqsort->sort(tmpArrpqs,size);
	//cout << "Sort rownolegly" << endl;
	for (int i = 0; i < size; i++)
		if (tmpArrqs[i] != tmpArrpqs[i]) {
			cout << "Not this same"<<endl;
			return false;
		}
	cout << "This same" << endl;
	delete[] tmpArrqs;
	delete[] tmpArrpqs;
	return true;
}
int main() {
	constexpr int SIZE = 5000;
	int * original_arr = generate(SIZE);
	//compare(original_arr, SIZE);
	//int bor = get_best_border(original_arr, SIZE);
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