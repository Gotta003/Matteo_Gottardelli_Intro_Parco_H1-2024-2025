//
//  main.c
//  Matrix Transposition
//
//  Created by Matteo Gottardelli on 16/11/24.
//
//Access to the same node (pbs script and insert select=1:ncpus=64:ompthreads=64:mem=1Gb
//export OMP_NUM_THREADS=1; ./exeC
//Too avoid outlayer
//qsub -I -q short_cpuQ -l select=1:ncpus=64:ompthreads=64:mem=1Gb
//NO -O3
#include <stdio.h>
#include "functions.h"

int main(int argc, const char * argv[]) {
    //Initialization
    int count=0;
    double time=0.0;
    float** M=NULL;
    float** T=NULL;
    //struct timeval start_tv, end_tv;
    //#ifdef _OPENMP
    #ifdef _OPENMP
        double tw_start=0.0, tw_end=0.0;
    #endif
    //#endif
    //Input parameters
    inputParameters(argc);
    const char* CODE=argv[1];
    const Mode MODE=valueInputed(argc, argv[2], 2);
    const int N=valueInputed(argc, argv[3], 3);
    const int TESTING=valueInputed(argc, argv[4], 4);
    const int SAMPLES=valueInputed(argc, argv[5], 5);
    const int NUM_THREADS=threadInit(argv[6], argc, MODE);
    if(N/2<MINIMUMSUBLENGTH) {
        fprintf(stderr, "Error, input dimension under the limit\n");
        exit(1);
    }
    int SUBLENGTH=MAX(MIN(N/2, MAXIMUMSUBLENGTH/NUM_THREADS), MINIMUMSUBLENGTH);
    printf("%d\n\n", SUBLENGTH);
    double* results=malloc(sizeof(double)*SAMPLES);
    if (results==NULL) {
        fprintf(stderr, "ERROR Memory Allocation");
        exit(1);
    }
    //Optimized Length Function
    while (count<SAMPLES) {
        //Allocation in memory
        clearAllCache();
        M=allocateMatrixPerMode(MODE, N);
        T=allocateMatrixPerMode(MODE, N);
        initializeMatrix(M, TESTING, N);
        //Starting
        //if(MODE>=OMP_LOC_WS && MODE<=OMP_GLB_TB) {
        #ifdef _OPENMP
                tw_start=omp_get_wtime();
        #endif
        //}
        //else {
        //    gettimeofday(&start_tv, NULL);
        //}
        //Modalities
        bool symmetric=executionProgram(M, T, MODE, N, SUBLENGTH);
        //control(M, T, N);
        //Ending
        //if(MODE>=OMP_LOC_WS && MODE<=OMP_GLB_TB) {
        #ifdef _OPENMP
            tw_end=omp_get_wtime();
            time=(tw_end-tw_start);
            printf("Time Elapsed (get time)=%.12f\n", time);
        #endif
        //}
        /*else {
            gettimeofday(&end_tv, NULL);
            time=end_tv.tv_sec-start_tv.tv_sec+(end_tv.tv_usec-start_tv.tv_usec)*1e-6;
            printf("Time Elapsed (get time)=%.6f\n", time);
        }*/
        //Free Memory
        freeMemory(M, N);
        if(!symmetric) {
            freeMemory(T, N);
        }
        //Incrementing
        openFilesResultsPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_THREADS, time);
        results[count]=time;
        count++;
    }
    //Printing
    //Take the 10 values in the middle
    const int TAKE_SAMPLES=SAMPLES/5*2;//40%
    int start=SAMPLES/2-TAKE_SAMPLES/2;
    int end=SAMPLES/2+TAKE_SAMPLES/2;
    double total_time=0.0;
    bubbleSort(results, SAMPLES);
    for (int i=start; i<end; i++) {
        total_time+=results[i];
        printf("%.12lf\n", results[i]);
    }
    printf("\n\nFINAL RESULTS WITH:\nMODE %d\nDimension: %d\nTesting: %d\nSamples: %d\nThreads: %d\nAverage Time: %.12lf secs\n\n", MODE, N, TESTING, SAMPLES, NUM_THREADS, total_time/TAKE_SAMPLES);
    openFilesAvgPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_THREADS, total_time/TAKE_SAMPLES);
    free(results);
    return 0;
}
