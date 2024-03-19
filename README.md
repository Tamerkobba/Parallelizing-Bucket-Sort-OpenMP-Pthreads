# Parallelizing Bucket Sort using OpenMp and Pthreads
Tamer Kobba
202104873
# Introduction

Bucket sort is an efficient sorting algorithm that distributes elements of an array into a number of buckets. Each bucket is then sorted individually, either using a different sorting algorithm or by recursively applying the bucket sort. Finally, the sorted buckets are merged to produce a sorted array. This approach is particularly effective for sorting data with a uniform distribution across a range.
```python
Algorithm Sequential Bucket Sorting Algorithm.
1: Initialize an array of empty “buckets”.
2: Go over the unsorted original array and put elements in 
the bucket based on their values.
3: Sort each non-empty buckets.
4: Visit the array of buckets in order and put the sorted 
elements back to the original array.
```
# How we parallelized the problem
## Techniques used
### Data Partitioning

The original array is divided into segments that are processed in parallel, with each thread working on a distinct subset of the data. This is evident in the division of the array into `N/K` segments for each thread, allowing simultaneous insertion of elements into buckets.

### Task Parallelism

The algorithm employs task parallelism by assigning different tasks (inserting into buckets and then sorting these buckets) to separate threads. This approach utilizes the cores of the processor efficiently by performing multiple operations concurrently.

### Synchronization

Critical sections where threads modify shared data structures (e.g., inserting elements into buckets) require synchronization to prevent race conditions. In the OpenMP implementation, this is achieved with the `#pragma omp critical` directive. In the pthreads implementation, mutex locks (`pthread_mutex_lock` and `pthread_mutex_unlock`) are used to synchronize access to the buckets.

## Pseudo code
- `N`: Number of unsorted elements
- `M`: Number of buckets
- `K`: Number of threads

**Algorithm Steps:**

1. **Initialize an array of empty “buckets”.**

2. **Partition the original array into `K` sets, each has `N/K` elements.**

3. **In parallel, each thread performs the following:**
   - Go over `N/K` elements in the original array and put them in the bucket based on their values.

4. **Partition buckets into `K` sets, each has `M/K` buckets.**

5. **In parallel, each thread performs the following:**
   - Sort `M/K` non-empty buckets.

6. **Visit the array of buckets in order and put the sorted elements back to the original array.**

# Code
[Github link](https://github.com/Tamerkobba/Parallelizing-Bucket-Sort-OpenMP-Pthreads).

# Performance 
All the versions were performed on array size `100000` with `10` buckets
## Sequential
Average Running time after 10 times=0.842438
## OpenMP
`on 2 threads`
Average Running time after 10 times=1.329058 seconds
`on 4 threads`
Average Running time after 10 times=1.263154 seconds
`on 8 threads`
Average Running time after 10 times= 1.283905 seconds
`on 16 threads`
Average Running time after 10 times= 0.759160 seconds
`on 32 threads`
Average Running time after 10 times=0.843396 seconds
`on 64 threads`
Average Running time after 10 times=0.847123 seconds
`on 128 threads`
Average Running time after 10 times=0.853252 seconds

| Processors | Speedup  | Efficiency (%)    |
|------------|----------|-------------------|
| 2          | 0.63386  | 31.693%           |
| 4          | 0.66693  | 16.67325%         |
| 8          | 0.65615  | 8.201875%         |
| 16         | 1.1096   | 6.935%            |
| 32         | 0.998864 | 3.12145%          |
| 64         | 0.994469 | 1.553857813%      |
| 128        | 0.987326 | 0.7713%           |
## pthreads
`on 2 threads`
Average Running time after 10 times=1.501783 seconds
`on 4 threads`
Average Running time after 10 times=1.631667 seconds
`on 8 threads`
Average Running time after 10 times=  1.428928 seconds
`on 16 threads`
Average Running time after 10 times= 0.909735 seconds
`on 32 threads`
Average Running time after 10 times=0.913971 seconds
`on 64 threads`
Average Running time after 10 times=0.868448 seconds
`on 128 threads`
Average Running time after 10 times= 0.924749 seconds

| Processors | Speedup  | Efficiency (%) |
|------------|----------|----------------|
| 2          | 0.56095  | 28.05          |
| 4          | 0.516305 | 12.91          |
| 8          | 0.589559 | 7.37           |
| 16         | 0.926025 | 5.79           |
| 32         | 0.921733 | 2.88           |
| 64         | 0.97005  | 1.52           |
| 128        | 0.91099  | 0.71           |


### Scalability
#### OpenMP
For the OpenMP implementation, the speedup and efficiency metrics indicate a non-linear scalability. The efficiency decreases as the number of processors increases, with a notable efficiency drop beyond 2 processors. This trend suggests diminishing returns in performance gains with the addition of more threads, particularly as the thread count goes beyond the physical or logical core count of the underlying hardware. Interestingly, there's a performance peak at 16 threads, where the speedup surpasses 1, indicating that this configuration somehow optimizes the workload distribution and CPU utilization for this specific task and dataset.

#### pthreads
The pthreads implementation shows a similar trend of decreasing efficiency with an increasing number of threads. However, the pthreads approach seems to maintain a more consistent performance improvement, evidenced by the speedup values being closer to 1 or above in the higher thread counts, particularly at 64 threads. This consistency might imply a slightly better handling of thread management or synchronization mechanisms specific to pthreads, but it still exhibits the same fundamental issue of reduced efficiency with scale.

# Comparison

## Similarities

1. **Parallel Decomposition Strategy**: Both implementations divide the sorting task into smaller sub-tasks that can be executed in parallel. This involves dividing the array into segments for bucket insertion and assigning specific buckets or groups of buckets to separate threads for sorting.

2. **Use of Buckets for Sorting**: They utilize the same algorithmic strategy of bucket sort, where elements are first distributed into a number of buckets based on their value, and then each bucket is sorted individually before merging the results.

3. **Dynamic Work Allocation**: Both approaches dynamically allocate segments of the array to different threads for initial bucket insertion. This workload division is based on the total number of elements (`N`) and the number of threads or processors (`K`).

4. **Synchronization Mechanism**: There's a need for synchronization in both implementations to ensure that shared data structures (i.e., buckets and bucket counts) are not accessed concurrently by multiple threads in a way that could lead to data corruption. OpenMP uses critical sections, while pthreads use mutexes for this purpose.

## Differences

1. **Thread Management**:
   - **OpenMP** abstracts away much of the complexity of thread creation, management, and destruction using compiler directives. It automatically handles the distribution of threads across available processors.
   - **Pthreads** requires explicit creation, management, and synchronization of threads through function calls. The programmer has more control but also bears the responsibility for thread lifecycle and synchronization details.

2. **Synchronization**:
   - **OpenMP** uses `#pragma omp critical` sections to protect code blocks that must not be executed by more than one thread at a time.
   - **Pthreads** achieves synchronization through explicit mutex locks and unlocks around critical sections, giving the programmer finer control over which parts of the code need synchronization and potentially leading to more efficient use of locks.

# Discussion and Conclusions
## Performance Analysis
### OpenMP 
With OpenMP, the only notable speedup occurs at 16 threads with a speedup of 1.1096, which is relatively marginal. This speedup indicates that while there is some benefit to parallel execution, it is significantly offset by the overheads associated with parallelization, such as synchronization, thread management, and potentially non-uniform workload distribution across threads. The efficiency percentages, which decrease as more threads are introduced, underscore the diminishing returns of adding more processing units beyond a certain point. At 16 threads, despite being the optimal setup among the tested configurations, the efficiency is only 6.935%, reflecting a substantial underutilization of the computational resources available.

### Pthreads
The pthreads implementation demonstrates a similar trend, with efficiency decreasing consistently as the number of threads increases.These results suggest that for the bucket sort algorithm, particularly with the given array size and number of buckets, the overheads associated with parallel computing—such as thread management and synchronization—negate the potential computational benefits.

## The Challenges of Parallelizing Bucket Sort
The bucket sort algorithm's performance in parallel computing environments is highly dependent on several factors, including the distribution of data across buckets and the efficiency of sorting within each bucket. If the data distribution results in an uneven workload across threads or if the overhead of managing parallel tasks outweighs the computational work, the benefits of parallelization can be minimal or even negative.

Furthermore, the modest speedup observed with OpenMP at 16 threads, though better than the sequential and other parallel configurations, highlights a crucial aspect of parallel computing: not all algorithms scale linearly with the addition of more processing units.
Additionally,in Hong, Hiep. "Parallel Bucket Sorting Algorithm," The paper's experiment results, showing some improvement in execution times with an increased number of processor cores but also pointing out the speedup limitations due to parallelization overheads, offer a concrete example that supports my observations.

## Conclusion

The exploration into parallelizing the bucket sort algorithm via OpenMP and pthreads reveals significant insights into the complexities and nuanced performance gains of parallel computation. Although the implementation with OpenMP demonstrated a marginal speedup at 16 threads, this gain is overshadowed by the parallelization overheads, including synchronization ,load balancing and thread management, leading to diminishing returns with increased thread counts.

The performance analysis aligns with Hiep Hong's findings in "Parallel Bucket Sorting Algorithm," where even with the scalability potential of multicore architectures, the speedup is modest and constrained by the fraction of the algorithm that benefits from parallelization. This underscores a fundamental principle in parallel computing: the extent of performance improvement through parallelization is intrinsically linked to the algorithm's nature, data distribution, and the efficiency of managing parallel tasks.

In conclusion the parallelization of the bucket sort algorithm with OpenMP and pthreads highlights the delicate balance between achieving computational speedups and managing the overheads inherent in parallel computing, affirming that the potential for performance improvements is intricately tied to addressing key challenges such as load balancing and synchronization.
Reference:
Hong, H. (n.d.). Parallel Bucket Sorting Algorithm. Computer Science Department, San Jose State University. Retrieved from[https://www.sjsu.edu/people/robert.chun/courses/cs159/s3/N.pdf](https://www.sjsu.edu/people/robert.chun/courses/cs159/s3/N.pdf). 