#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 100000
#define M 10
#define K 128
void bucket_sort_parallel(int arr[], int n);
void insert_into_bucket(int arr[], int start, int end, int buckets[][N], int bucket_counts[], int range);
void sort_buckets(int buckets[][N], int bucket_counts[], int start, int end);

int main() {
    int arr[N];
     clock_t start, end;
    double total_cpu_time_used = 0.0;
 for (int iter = 0; iter < 10; iter++) {
  srand(42);
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 100; 
    }

   

   
        start = clock();
        bucket_sort_parallel(arr, N);
        end = clock();

        double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        total_cpu_time_used += cpu_time_used;

        
        
             
         
         

        printf("Iteration %d: Time taken: %f seconds\n", iter+1, cpu_time_used);
    }

    printf("Average running time over 10 iterations: %f seconds\n", total_cpu_time_used / 10);

    return 0;
}

void bucket_sort_parallel(int arr[], int n) {
    int buckets[M][N] = {0}; 
    int bucket_counts[M] = {0}; 
    int range = 100 / M; 

    
    #pragma omp parallel num_threads(K)
    {
        int thread_id = omp_get_thread_num();
        int items_per_thread = n / K;
        int start = thread_id * items_per_thread;
        int end = start + items_per_thread - 1;
        if (thread_id == K - 1) end = n - 1; 
        insert_into_bucket(arr, start, end, buckets, bucket_counts, range);
    }

    
    #pragma omp parallel num_threads(K)
    {
        int thread_id = omp_get_thread_num();
        int buckets_per_thread = M / K;
        int start = thread_id * buckets_per_thread;
        int end = start + buckets_per_thread - 1;
        if (thread_id == K - 1) end = M - 1; 
        sort_buckets(buckets, bucket_counts, start, end);
    }

    
    int index = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < bucket_counts[i]; j++) {
            arr[index++] = buckets[i][j];
        }
    }
}

void insert_into_bucket(int arr[], int start, int end, int buckets[][N], int bucket_counts[], int range) {
    for (int i = start; i <= end; i++) {
        int bucket_index = arr[i] / range;
        #pragma omp critical
        {
            buckets[bucket_index][bucket_counts[bucket_index]++] = arr[i];
        }
    }
}

void sort_buckets(int buckets[][N], int bucket_counts[], int start, int end) {
    for (int i = start; i <= end; i++) {
        if (bucket_counts[i] > 0) {
            
            for (int j = 1; j < bucket_counts[i]; j++) {
                int key = buckets[i][j];
                int k = j - 1;
                while (k >= 0 && buckets[i][k] > key) {
                    buckets[i][k + 1] = buckets[i][k];
                    k = k - 1;
                }
                buckets[i][k + 1] = key;
            }
        }
    }
}
