#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <pthread.h>
#include <sched.h>
#include <assert.h>

typedef struct
{
    double begin;
    double delta;
    double integral;
    double end;
} thread_data;

const      double        BEGIN       = 1;
const      double        RANGE       = 1 << 27;
const long long unsigned SAMPLES_NUM = ((long long int)1) << 31;

void* compute_integral(void* arg);
void* extra_thread(void* arg);

int main(int argc, char** argv)
{
    // Declarations and initializations
    int cores_num = 0;
    pthread_t   threads[32] = {0};
    pthread_t   extra_threads[32] = {0};
    thread_data args   [32] = {0};

    cpu_set_t cpu_set;       CPU_ZERO(&cpu_set);
    cpu_set_t busy_core_set; CPU_ZERO(&busy_core_set);

    pthread_attr_t pta;
    int pai_ret = pthread_attr_init(&pta);
    assert(pai_ret != -1);

    FILE* cpuinfo_file = NULL;

    // Handle argument
    if (argc != 2)
    {
        printf("Usage: ./error_function N_CORES\n");
        exit(1);
    }

    cores_num = atoi(argv[1]);

    // Check number of cores
    if (cores_num > get_nprocs() / 2) //< Taking in account hyperthreading
    {
        printf("Not enough physical cores.\nAborting\n");
        exit(1);
    }

    // Share interval among threads
    double threadwise_range = RANGE / cores_num;
    for (int i = 0; i < cores_num; i++)
    {
        args[i].begin = BEGIN + threadwise_range * i;
        args[i].delta = RANGE / SAMPLES_NUM;
        args[i].end   = BEGIN + threadwise_range * (i + 1);
    }

    // Set affinity and create threads
    cpuinfo_file = fopen("/proc/cpuinfo", "r");
    assert(cpuinfo_file);
    int cpu_num = 0, core_num = 0;

    for (int i = 0; i < cores_num; i++)
    {
        //Choose physical cpu
        CPU_ZERO(&cpu_set);
        int is_core = 0;
        while (fscanf(cpuinfo_file, "%*[^c]core id%n", &is_core) != EOF)
        {
            if (!is_core)
            {
                continue;
            }
            fscanf(cpuinfo_file, "%*[^0-9]%d", &core_num);
            if (!CPU_ISSET(core_num, &busy_core_set))
            {
                printf("core_num = %d; cpu_num = %d\n", core_num, cpu_num);
                CPU_SET(cpu_num++, &cpu_set);
                CPU_SET(core_num,  &busy_core_set);
                break;
            }
            is_core = 0;
            cpu_num++;
        }
        //Set affinity
        int psn_ret = pthread_attr_setaffinity_np(&pta, sizeof(cpu_set_t), &cpu_set);
        assert(psn_ret != -1);
        //Create threads
        int ptc_ret = pthread_create(&threads[i], &pta,
                                     compute_integral, &args[i]);
        assert(ptc_ret != -1);
        ptc_ret = pthread_create(&extra_threads[i], &pta,
                                     extra_thread, NULL);
        assert(ptc_ret != -1);
    }
    fclose(cpuinfo_file);

    // Wait for threads to compute integral
    double integral = 0;
    for (int i = 0; i < cores_num; i++)
    {
        pthread_join(threads[i], NULL);
        integral += args[i].integral;
    }
    printf("\tIntegral = %20.15lg\n", integral);

    return 0;
}

#pragma GCC optimize ("O0")

void* extra_thread(void* arg)
{
    while (1);
    return NULL;
}

#pragma GCC optimize ("O3")

void* compute_integral(void* arg)
{
    register double begin = ((thread_data*)arg)->begin;
    register double end   = ((thread_data*)arg)->end;
    register double delta = ((thread_data*)arg)->delta;
    register double x     = begin;
    register double sum   = 0;

    while (x < end)
    {
        sum += 1 / x;
        x += delta;
    }

    ((thread_data*)arg)->integral = sum * delta;

    pthread_exit(0);
}
