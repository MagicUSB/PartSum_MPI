#include <stdio.h>
#include "mpi.h"
#include <iostream>
#include <vector>

#define ARR_SIZE 100000

using namespace std;

void init_array(vector<int>& arr) {
	for (size_t i = 0; i < arr.size(); i++)
	{
		arr[i] = i + 1;
	}
}

long long part_sum(vector<int>& arr, int start_index, int end_index) {
	long long result_sum = 0;
	for (int i = start_index; i < end_index; i++)
	{
		result_sum += arr[i];
	}
	return result_sum;
}

int main(int *argc, char **argv)
{
    int numtasks, rank;
	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	vector<int> arr(ARR_SIZE);
	init_array(arr);
	long long sum;
	int step;
	if (ARR_SIZE < 100)
	{
		step = ARR_SIZE > 1 ? ARR_SIZE / (log(ARR_SIZE) / log(numtasks)) : 1;
	}
	else
	{
		step = ARR_SIZE > numtasks ? ARR_SIZE / numtasks : 1;
	}
	int next = (rank * step + step) > ARR_SIZE ? ARR_SIZE : rank * step + step;
	if (rank == numtasks - 1)
	{
		next += ARR_SIZE % numtasks;
	}
	long long p_sum = part_sum(arr, rank * step, next);
	//cout << rank << " " << rank * step << " " << rank * step + step << endl;
	MPI_Reduce(&p_sum, &sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Finalize();

	if (rank == 0)
	{
		cout << "Sum: " << sum << endl;
	}
	return 0;

}
