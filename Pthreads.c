#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 100000
#define M 10
#define K 2

int arr[N];
int buckets[M][N] = {0};
int bucket_counts[M] = {0};
int range = 100 / M;
pthread_mutex_t lock[M];

typedef struct {
    int start;
    int end;
    int thread_id;
} thread_data;

void* insert_into_bucket(void* arg);
void* sort_buckets(void* arg);

int main() {
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 100;
    }

    pthread_t threads[K];
    thread_data t_data[K];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    clock_t start, end;
    double cpu_time_used;

    start = clock();

    // Initialize mutexes
    for (int i = 0; i < M; i++) {
        pthread_mutex_init(&lock[i], NULL);
    }

    // Creating threads for inserting into buckets
    for (int i = 0; i < K; i++) {
        t_data[i].start = i * (N / K);
        t_data[i].end = (i + 1) * (N / K) - 1;
        if (i == K - 1) t_data[i].end = N - 1; // Last thread may take more
        t_data[i].thread_id = i;

        pthread_create(&threads[i], &attr, insert_into_bucket, (void*)&t_data[i]);
    }

    // Joining threads after insertion
    for (int i = 0; i < K; i++) {
        pthread_join(threads[i], NULL);
    }

    // Creating threads for sorting buckets
    for (int i = 0; i < K; i++) {
        t_data[i].start = i * (M / K);
        t_data[i].end = (i + 1) * (M / K) - 1;
        if (i == K - 1) t_data[i].end = M - 1; // Last thread may take more

        pthread_create(&threads[i], &attr, sort_buckets, (void*)&t_data[i]);
    }

    // Joining threads after sorting
    for (int i = 0; i < K; i++) {
        pthread_join(threads[i], NULL);
    }

    // Merge sorted buckets
    int index = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < bucket_counts[i]; j++) {
            arr[index++] = buckets[i][j];
        }
    }

    end = clock();
      // Print sorted array
    //for (int i = 0; i < N; i++) {
        //printf("%d ", arr[i]);
    //}
    //printf("\n");

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);

    // Destroy mutexes
    for (int i = 0; i < M; i++) {
        pthread_mutex_destroy(&lock[i]);
    }
 
    return 0;
}

void* insert_into_bucket(void* arg) {
    thread_data* t_data = (thread_data*)arg;
    int start = t_data->start;
    int end = t_data->end;

    for (int i = start; i <= end; i++) {
        int bucket_index = arr[i] / range;
        pthread_mutex_lock(&lock[bucket_index]);
        buckets[bucket_index][bucket_counts[bucket_index]++] = arr[i];
        pthread_mutex_unlock(&lock[bucket_index]);
    }

    pthread_exit(NULL);
}

void* sort_buckets(void* arg) {
    thread_data* t_data = (thread_data*)arg;
    int start = t_data->start;
    int end = t_data->end;

    for (int i = start; i <= end; i++) {
        if (bucket_counts[i] > 0) {
            // Insertion sort
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

    pthread_exit(NULL);
}
