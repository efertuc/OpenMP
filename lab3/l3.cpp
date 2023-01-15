
На основе двух равно размерных массивов A и B(длины N) функция возвращает сумму значений, больших 1.

#include <stdio.h>
#include <omp.h>
#include <iostream>

using namespace std;

const int NMAX = 10000000;

void main()
{
	setlocale(LC_ALL, "rus");
	
	/*cout << "n :" << NMAX << endl;*/
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

	#pragma omp parallel shared(A,B) 
		{
	#pragma omp for  firstprivate(i, sum) reduction(+: total)
			for (i = 0; i < NMAX; i++)
			{
				sum = 0;
				if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1)
					sum += A[i] + B[i];
				else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1)
					sum += 4 * A[i] - B[i];
				total += sum;
			}
		}

		printf("\nsum: %d", total);
		printf("\nВремя: %lf\n", omp_get_wtime() - start_time);

		//-------------------------------------------------------------------------------------
	total = 0;
	//cout << "-----------------------------------" << endl;
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
