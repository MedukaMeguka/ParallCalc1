#include "stdafx.h"
#include<iostream>
#include <Windows.h>
#include <omp.h>
#include <iomanip>
#include <ctime>

using namespace std;

//Вывести параллельно “HelloWorld” 5 - 10 раз
void task1();
/*Исследовать влияние настроек параметра shedule (не менее 4 опций ) парадигмы
#omp parallel for. Вывести время выполнения циклов для:
 Итераций с равномерным временем выполнения;
 Итераций с неравномерным временем выполнения – нечетные итерации
выполняются дольше.
 Итераций с неравномерным временем выполнения –итерации в первой половине
цикла выполняются дольше.
Результат вывести в виде таблице (4 х 3). Объяснить результат.*/
void task2();
/*Провести суммирование элементов массива с использованием критических секций
(critical) и параметра reduction. Сравнить время работы, результат объяснить.*/
void task3();
//Придумать пример на барьерную синхронизацию #pragma omp barrier
void task4();
/*Распараллелить цикл:
For(i=2;i&lt;N;i++)
For(j=2;i&lt;N;j++)
A[i,j] =A[i-2,j] +A[i,j-2];*/
void task5();

int main()
{
	setlocale(LC_ALL, "rus");
	//cout << "Первое задание:\n"; task1();
	//cout << "Второе задание:\n"; task2();
	//cout << "Третье задание:\n"; task3();
	//cout << "Четвёртое задание:\n"; task4();
	cout << "Пятое задание:\n"; task5();

	cin.get();
}

void task1() {
#pragma omp parallel num_threads(10)
	{ cout << "Hello\n"; }
	printf("\n");
}

void iter(int t, int i, int N) {
	if (t == 0) {
		Sleep(5);
	}
	if (t == 1) {
		if (i % 2 == 0) Sleep(5);
	}
	if (t == 2) {
		if (i < N / 2) Sleep(5);
	}
}

void leftside(int t) {
	if (t == 0) cout << "равномерно            ";
	else if (t == 1) cout << "нечетные дольше       ";
	else cout << "первая половина дольше";
}



void task2() {
	cout << "                       "
		<< setw(10) << "static  "
		<< setw(10) << "dynamic  "
		<< setw(10) << "dynamic 3  "
		<< setw(10) << "guided  "
		<< endl;

	for (int i = 0; i < 3; i++) {
		int k = 100;
		unsigned int start_time, end_time;
		leftside(i);

		start_time = clock();
#pragma omp for schedule(static)
		for (int n = 0; n < k; n++) iter(i, n, k);
		end_time = clock();
		cout << setw(6) << end_time - start_time;

		start_time = clock();
#pragma omp for schedule(dynamic)
		for (int n = 0; n < k; n++) iter(i, n, k);
		end_time = clock();
		cout << setw(10) << end_time - start_time;

		start_time = clock();
#pragma omp for schedule(dynamic, 3)
		for (int n = 0; n < k; n++) iter(i, n, k);
		end_time = clock();
		cout << setw(10) << end_time - start_time;

		start_time = clock();
#pragma omp for schedule(guided)
		for (int n = 0; n < k; n++) iter(i, n, k);
		end_time = clock();
		cout << setw(10) << end_time - start_time;

		printf("\n\n");
	}
}

void task3() {
	int k = 10000;
	int* N = new int[k];
	unsigned int start_time, end_time;
	int sum = 0;
	for (int i = 0; i < k; i++) {
		N[i] = i;
	}



	start_time = clock();
#pragma omp critical
	{
		for (int i = 0; i < k; i++) sum += N[i];
	}
	end_time = clock();
	cout << "Результат " << sum << " " << "Время " << end_time - start_time << endl;

	sum = 0;
	start_time = clock();
#pragma omp parallel for reduction(+:sum)
	for (int i = 0; i < k; i++) sum += N[i];
	end_time = clock();
	cout << "Результат " << sum << " " << "Время " << end_time - start_time << endl;

}


void task4() {
	int changevar = 100;
#pragma omp parallel num_threads(2)
	{
		changevar++;
#pragma omp barrier
#pragma omp single
		{
			cout << changevar << endl;
		}
	}
}

void task5() {
	int count = 4;
	int **arr = new int*[count];
	int i, j;
	for (i = 0; i < count; i++) {
		arr[i] = new int[count];
		for (j = 0; j < count; j++) {
			arr[i][j] = i + j;
		}
	}
#pragma omp parallel for shared(arr) private(i,j)
	for (i = 2; i < count; i++)
		for (j = 2; j < count; j++)
			arr[i][j] = arr[i - 2][j] + arr[i][j - 2];

	for (i = 0; i < count; i++) {
		for (j = 0; j < count; j++)
			printf("%4d", arr[i][j]);
		printf("\n");
	}
	printf("\n");
}