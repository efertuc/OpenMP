
#include <stdio.h>
#include <omp.h>
#include <locale.h>
#include <time.h>
#include <clocale>
#include <iostream>
using namespace std;

int* init(int N) {
	int* arr = new int[N];
	for (int i = 0; i < N; i++) {
		arr[i] = rand() % 100 - 50;
	}
	return (arr);
}


void lab3(int* A, int* B, int N, int limit) {
	double start_time, end_time;
	int total = 0;
	start_time = omp_get_wtime();
#pragma omp parallel shared(A, B, N, total) if(N > limit)
	{

#pragma omp sections reduction(+: total) 
		{
				for (int i = 0; i < N ; i++)
				{
					if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
						total += A[i] + B[i];
					else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
						total += 4 * A[i] - B[i];
				}
		}
	}
	end_time = omp_get_wtime();
	printf("lab 3\n");
	/*printf("sum- %d\n", total);*/
	printf("N - %d\nlimit - %d\n", N, limit);
	printf("time - %lf\n\n", end_time - start_time);
}

void section_2(int* A, int* B, int N, int limit) {
	double start_time, end_time; 
	int total = 0;
	start_time = omp_get_wtime();
#pragma omp parallel shared(A, B, N, total) if(N > limit)
	{
		
#pragma omp sections reduction(+: total) 
		{
#pragma omp section
			{
				for (int i = 0; i < N / 2; i++)
				{
					if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
						total += A[i] + B[i];
					else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
						total += 4 * A[i] - B[i];
				}
			}
#pragma omp section 
			{
				for (int i = N / 2; i < N; i++)
				{
					if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
						total += A[i] + B[i];
					else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
						total += 4 * A[i] - B[i];
				}
			}
		}
	}
	end_time = omp_get_wtime(); 
	printf("2 sect\n");
	/*printf("sum- %d\n", total);*/
	printf("N - %d\nlimit - %d\n", N, limit);
	printf("time - %lf\n\n", end_time - start_time);
}

void section_4(int* A, int* B, int N, int limit)
{
	int total = 0;
	double start_time, end_time; 
	start_time = omp_get_wtime(); 
#pragma omp parallel shared(A, B, N, total) if(N > limit)
	{
		
#pragma omp sections reduction(+: total) // 4 секции
		{
#pragma omp section
			{
				for (int i = 0; i < N / 4; i++)
				{
					if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
						total += A[i] + B[i];
					else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
						total += 4 * A[i] - B[i];
				}
			}

#pragma omp section 
			{
				for (int i = N / 4; i < N / 2; i++)
				{
					if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
						total += A[i] + B[i];
					else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
						total += 4 * A[i] - B[i];
				}
			}
#pragma omp section 
			{
				for (int i = N / 2; i < N * 3 / 4; i++)
				{
					if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
						total += A[i] + B[i];
					else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
						total += 4 * A[i] - B[i];
				}
			}
#pragma omp section 
			{
				for (int i = N * 3 / 4; i < N; i++)
				{
					if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
						total += A[i] + B[i];
					else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
						total += 4 * A[i] - B[i];
				}
			}
		}

	}
	end_time = omp_get_wtime(); //  онец
	printf("4 section\n");
	/*printf("sum - %d\n", total);*/
	printf("N - %d\nlimit - %d\n", N, limit);
	printf("time - %lf\n\n", end_time - start_time);
}



void lab5(int N, int M) {
	for (int i = M; i < N; i += M)
	{
		int* A = init(i);
		int* B = init(i);
		lab3(A, B, i, 1);
		section_2(A, B, i, 1); // параллельно
		section_2(A, B, i, i + 1); // последовательно
		section_4(A, B, i, 1); // параллельно
		section_4(A, B, i, i + 1); // последовательно
		delete[] A;
		delete[] B;
	}
}

void main()
{
	setlocale(LC_ALL, "rus");
	lab5(100001, 20000);
}
