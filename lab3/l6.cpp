#include <stdio.h>
#include <omp.h>
#include <iostream>

int* init(int N) {
	int* arr = new int[N];
	for (int i = 0; i < N; i++) {
		arr[i] = rand() % 100 - 50;
	}
	return (arr);
}

void lock(int* A, int* B, int N, int limit)
{
	double start_time, end_time; 
	int total = 0;
	omp_lock_t lock;
	omp_init_lock(&lock); // »нициализаци€ замка
	start_time = omp_get_wtime(); 
#pragma omp parallel shared(A, B, N, total) if(N > limit)
	{
		// ≈сли (N>LIMIT) не выполнитс€, то
		// будет обычный последовательный код

#pragma omp for

		for (int i = 0; i < N; i++)
		{

			if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1) {
				omp_set_lock(&lock);
				total += A[i] + B[i];
				omp_unset_lock(&lock);
			}
									
			else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1) {
				omp_set_lock(&lock);
				total += 4 * A[i] - B[i];
				omp_unset_lock(&lock);
			}
		}
	}
	end_time = omp_get_wtime(); //  онец
	omp_destroy_lock(&lock); // ѕереводим замок в неиниц. состо€ние
	printf("lock\n");
	printf("sum- %d\n", total);
	printf("N - %d\nlimit - %d\n", N, limit);
	printf("time - %lf\n\n", end_time - start_time);
}

void barr(int* A, int* B, int N, int limit)
{
	double start_time, end_time; 
	int total = 0, ind;
	int n = omp_get_max_threads(); // создаем массив дл€ всех потоков
	int* arr = new int[n];
	for (int i = 0; i < n; i++)
	{
		arr[i] = 0;
	}
	start_time = omp_get_wtime(); 

#pragma omp parallel shared(A, B, arr, n, total) if(N > limit)
	{
		
#pragma omp for private(ind) // дл€ каждой нити записываем значение в массив

		for (int i = 0; i < N; i++)
		{
			ind = omp_get_thread_num();

			if ((A[i] + B[i]) > (4 * A[i] - B[i]) && (A[i] + B[i]) > 1) {
				arr[ind] += A[i] + B[i];
			}

			else if ((A[i] + B[i]) < (4 * A[i] - B[i]) && (4 * A[i] - B[i]) > 1) {
				arr[ind] += 4 * A[i] - B[i];
			}
		}
#pragma omp single nowait

		// ждем когда все потоки достигнут этой точки, происходит
		// не€вна€ барьерна€ синхронизаци€ с помощью nowait
		// single нужен, чтобы этот участок кода произошЄл ровно 1 раз
		for (int i = 0; i < n; i++)
		{
			total += arr[i];
		}
	}
	end_time = omp_get_wtime(); //  онец
	printf("barr\n");
	printf("sum - %d\n", total);
	printf("N - %d\nlimit - %d\n", N, limit);
	printf("time - %lf\n\n", end_time - start_time);
}

void lab6(int N, int M)
{
	for (int i = M; i < N; i += M)
	{
		int* A = init(i);
		int* B = init(i);
		
		lock(A, B, i, 1);// параллельно
		lock(A, B, i, i + 1); // последовательно
		barr(A, B, i, 1); // параллельно
		barr(A, B, i, i + 1); // последовательно
		delete[] A;
		delete[] B;
		
	}
}

int main(int argc, char* argv[])
{
	lab6(100001, 20000);
	
}