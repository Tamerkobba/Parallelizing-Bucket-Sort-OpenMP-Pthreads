#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000
#define M 10

void bucket_sort_serial(int arr[], int n);
void insert_into_bucket(int arr[], int start, int end, int buckets[][N], int bucket_counts[], int range);
void sort_buckets(int buckets[][N], int bucket_counts[], int start, int end);

int main() {
    int arr[N];

    
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 100; 
    }

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    
    bucket_sort_serial(arr, N);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    
    for (int i = 0; i < N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("Time taken: %f seconds\n", cpu_time_used);

    return 0;
}

void bucket_sort_serial(int arr[], int n) {
    int buckets[M][N] = {0};
    int bucket_counts[M] = {0};
    int range = 100 / M;

    
    insert_into_bucket(arr, 0, n - 1, buckets, bucket_counts, range);

    
    sort_buckets(buckets, bucket_counts, 0, M - 1);

    
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
        buckets[bucket_index][bucket_counts[bucket_index]++] = arr[i];
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
