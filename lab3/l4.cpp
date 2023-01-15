
#include <stdio.h>
#include <omp.h>
#include <iostream>
using namespace std;

const int NMAX = 100;

int* init(int N) {
	int* arr = new int[N];
	for (int i = 0; i < N; i++) {
		arr[i] = rand() % 100 - 50;
	}
	return (arr);
}

void atomic(int* A, int* B, int N, int limit)
{
	double start_time, end_time;
	int total = 0;
	int sum = 0;
	int i;

	start_time = omp_get_wtime();
#pragma omp parallel shared(A,B,total) 
	{
#pragma omp for  firstprivate(i, sum) 
		for ( i = 0; i < NMAX; i++)
		{
			sum = 0;
			if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
				sum += A[i] + B[i];
			else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
				sum += 4 * A[i] - B[i];
#pragma omp atomic
			total += sum;
		}
	}
	end_time = omp_get_wtime(); // Конец
	printf("atomic\n");
	/*printf("sum- %d\n", total);*/
	printf("N - %d\nlimit - %d\n", N, limit);
	printf("time - %lf\n\n", end_time - start_time);
}

void critical(int* A, int* B, int N, int limit)
{
	double start_time, end_time;
	int total = 0;
	int sum = 0;
	int i;
	start_time = omp_get_wtime();
#pragma omp parallel shared(A,B,total) 
	{
#pragma omp for  firstprivate(i, sum) 
		for ( i = 0; i < NMAX; i++)
		{
			sum = 0;
			if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
				sum += A[i] + B[i];
			else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
				sum += 4 * A[i] - B[i];
#pragma omp critical
			total += sum;
		}
	}
	end_time = omp_get_wtime(); // Конец
	printf("critical\n");
	/*printf("sum- %d\n", total);*/
	printf("N - %d\nlimit - %d\n", N, limit);
	printf("time - %lf\n\n", end_time - start_time);
}

void lab4(int N, int M)
{
	for (int i = M; i < N; i += M)
	{
		int* A = init(i);
		int* B = init(i);

		atomic(A, B, i, 1);// параллельно
		atomic(A, B, i, i + 1); // последовательно
		critical(A, B, i, 1); // параллельно
		critical(A, B, i, i + 1); // последовательно
		delete[] A;
		delete[] B;
	}
}

void main()
{
	setlocale(LC_ALL, "rus");
	lab4(100001, 20000);
	cout << "n :" << NMAX << endl;
	int i, j;
	long long int total = 0;
	long long int sum = 0;

	int* A = new int[NMAX];
	int* B = new int[NMAX];

	for (i = 0; i < NMAX; i++) {
		A[i] = rand() % 100 - 50;
		B[i] = rand() % 100 - 30;
	}

	double start_time = omp_get_wtime();

#pragma omp parallel shared(A,B,total) 
	{
#pragma omp for  firstprivate(i, sum) 
		for (i = 0; i < NMAX; i++)
		{
			sum = 0;
			if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
				sum += A[i] + B[i];
			else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
				sum += 4 * A[i] - B[i];
			#pragma omp atomic
				total += sum;
		}
	}

	printf("\nsum: %d", total);
	printf("\nВремя atomic: %lf\n", omp_get_wtime() - start_time);


	total = 0;
	double start_time2 = omp_get_wtime();

#pragma omp parallel shared(A,B,total) 
	{
#pragma omp for  firstprivate(i, sum) 
		for (i = 0; i < NMAX; i++)
		{
			sum = 0;
			if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
				sum += A[i] + B[i];
			else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
				sum += 4 * A[i] - B[i];
#pragma omp critical
			total += sum;
		}
	}

	printf("\nsum: %d", total);
	printf("\nВремя critical: %lf\n", omp_get_wtime() - start_time2);

	//-------------------------------------------------------------------------------------
	total = 0;
	cout << "-----------------------------------" << endl;
	double start_time1 = omp_get_wtime();
	for (i = 0; i < NMAX; i++)
	{
		if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
			total += A[i] + B[i];
		else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
			total += 4 * A[i] - B[i];
	}

	printf("\nsum: %d", total);
	printf("\nВремя: %lf\n", omp_get_wtime() - start_time1);
	//--------------------------------------------------------------------------------------
	delete[] A, B;
}
