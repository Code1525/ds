#include<stdio.h>
#include<mpi.h>
#define arr_size 15
int main(int argc, char *argv[]){
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//Code that will execute inside process 0 or rank 0
	if(rank == 0){
		int arr[]= {12,4,6,3,21,15,3,5,7,8,9,1,5,3,5};
		int global_sum = 0, local_sum = 0, recv_local_sum;
		//If the array size is perfectly divisible by number of process.
		if(arr_size%size == 0){
			int array_element_per_process = arr_size/size;
			int sub_arr[array_element_per_process];
			for(int i=1; i<size; i++){
				//Copying the sub array
				for(int j=0; j<array_element_per_process;j++){
					sub_arr[j] = arr[i*array_element_per_process+j];
				}			
				//Sending array chunk of equal size to all the process.
				MPI_Send(sub_arr, array_element_per_process, MPI_INT, i, 1, MPI_COMM_WORLD);
				MPI_Send(&array_element_per_process, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			}
			//Calculating the local sum of rank 0 itself
			for(int j=0; j<array_element_per_process; j++){
				local_sum += arr[j];
			}
			printf("Rank %d: local sum: %d\n", rank, local_sum);
			global_sum += local_sum;
		//When the array size is not perfectly divisible by number of process.	
		}else{
			int array_element_per_process = arr_size/size + 1;
			int sub_arr[array_element_per_process];
			for(int i=1; i<size; i++){
				if(i == size - 1){
					//last sub array will have the size less than other process array size
					int total_array_size_of_last_process = arr_size - array_element_per_process * i;
					for(int j=0; j< total_array_size_of_last_process; j++){
						sub_arr[j] = arr[i*array_element_per_process+j];
					}
					MPI_Send(&sub_arr, total_array_size_of_last_process, MPI_INT, i, 1, MPI_COMM_WORLD);
					MPI_Send(&total_array_size_of_last_process, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
				}else{
					//Copying the sub array
					for(int j=0; j<array_element_per_process;j++){
						sub_arr[j] = arr[i*array_element_per_process+j];
					}				
					MPI_Send(&sub_arr, array_element_per_process, MPI_INT, i, 1, MPI_COMM_WORLD);
					MPI_Send(&array_element_per_process, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
				}
			}
			//Calculating the local sum of rank 0 itself
			for(int j=0; j<array_element_per_process; j++){
				local_sum += arr[j];
			}
			printf("Rank %d: local sum: %d\n", rank, local_sum);
			global_sum += local_sum;
		}
		//calculating the global sum of the array
		//Receving the local sum from the other process and updating the global sum
		for(int i=1; i<size; i++){
			MPI_Recv(&recv_local_sum, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			global_sum += recv_local_sum;
		}
		//Printing the output
		printf("The sum of the array is %d\n", global_sum);
	//Code that will get executed inside other than process 0 or rank 0.
	}else{
		//The other process will receive the chunck of array
		int array_element_per_process = arr_size/size + 1;
		int recv_sub_arr[array_element_per_process];
		int recv_array_element_per_process, local_sum = 0;

		MPI_Recv(recv_sub_arr, recv_array_element_per_process, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&recv_array_element_per_process, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//Calculating local sum for the sub array
		for(int j=0; j<recv_array_element_per_process; j++){
			local_sum += recv_sub_arr[j];
		}
		//Printing the local sum
		printf("Rank %d: local sum: %d\n", rank, local_sum);
		//Sending back the local sum to the rank 0 or process 0.
		MPI_Send(&local_sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}