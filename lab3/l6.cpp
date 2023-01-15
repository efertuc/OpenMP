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
	omp_init_lock(&lock); // ������������� �����
	start_time = omp_get_wtime(); 
#pragma omp parallel shared(A, B, N, total) if(N > limit)
	{
		// ���� (N>LIMIT) �� ����������, ��
		// ����� ������� ���������������� ���

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
	end_time = omp_get_wtime(); // �����
	omp_destroy_lock(&lock); // ��������� ����� � ������. ���������
	printf("lock\n");
	printf("sum- %d\n", total);
	printf("N - %d\nlimit - %d\n", N, limit);
	printf("time - %lf\n\n", end_time - start_time);
}

void barr(int* A, int* B, int N, int limit)
{
	double start_time, end_time; 
	int total = 0, ind;
	int n = omp_get_max_threads(); // ������� ������ ��� ���� �������
	int* arr = new int[n];
	for (int i = 0; i < n; i++)
	{
		arr[i] = 0;
	}
	start_time = omp_get_wtime(); 

#pragma omp parallel shared(A, B, arr, n, total) if(N > limit)
	{
		
#pragma omp for private(ind) // ��� ������ ���� ���������� �������� � ������

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

		// ���� ����� ��� ������ ��������� ���� �����, ����������
		// ������� ��������� ������������� � ������� nowait
		// single �����, ����� ���� ������� ���� ��������� ����� 1 ���
		for (int i = 0; i < n; i++)
		{
			total += arr[i];
		}
	}
	end_time = omp_get_wtime(); // �����
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
		
		lock(A, B, i, 1);// �����������
		lock(A, B, i, i + 1); // ���������������
		barr(A, B, i, 1); // �����������
		barr(A, B, i, i + 1); // ���������������
		delete[] A;
		delete[] B;
		
	}
}

int main(int argc, char* argv[])
{
	lab6(100001, 20000);
	
}