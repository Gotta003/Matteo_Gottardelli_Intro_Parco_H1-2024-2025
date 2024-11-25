//
//  functions.h
//  Matrix Transposition
//
//  Created by Matteo Gottardelli on 16/11/24.
//

#ifndef functions_h
#define functions_h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/time.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <time.h>
#include <string.h>
#include <math.h> //-lm
#define KB 1024
#define CACHESIZEL1D (32*KB)
#define CACHESIZEL1I (32*KB)
#define CACHESIZEL2 (1024*KB)
#define CACHESIZEL3 (36608*KB)
#define MINIMUMSUBLENGTH pow(2, 4)/2
#define MAXIMUMSUBLENGTH sqrt(CACHESIZEL1D/(2*sizeof(float)))
#define FILENAMETGEN "timesGeneral.csv"
#define FILENAMETSEQ "timesSequential.csv"
#define FILENAMETIMPSTAND "timesImpStandard.csv"
#define FILENAMETIMPREC "timesImpRecursive.csv"
#define FILENAMETOMPLOCWS "timesOMPLocWS.csv"
#define FILENAMETOMPGLBWS "timesOMPGlbWS.csv"
#define FILENAMETOMPLOCBB "timesOMPLocBB.csv"
#define FILENAMETOMPGLBBB "timesOMPGlbBB.csv"
//#define FILENAMETOMPLOCTB "timesOMPLocTB.csv"
//#define FILENAMETOMPGLBTB "timesOMPGlbTB.csv"
#define FILENAMEGEN "resultsGeneral.csv"
#define FILENAMESEQ "resultsSequential.csv"
#define FILENAMEIMPSTAND "resultsImpStandard.csv"
#define FILENAMEIMPREC "resultsImpRecursive.csv"
#define FILENAMEOMPLOCWS "resultsOMPLocWS.csv"
#define FILENAMEOMPGLBWS "resultsOMPGlbWS.csv"
#define FILENAMEOMPLOCBB "resultsOMPLocBB.csv"
#define FILENAMEOMPGLBBB "resultsOMPGlbBB.csv"
//#define FILENAMEOMPLOCTB "resultsOMPLocTB.csv"
//#define FILENAMEOMPGLBTB "resultsOMPGlbTB.csv"
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX_THREADS 64
#define MIN_SIZE pow(2, 4)
#define MAX_SIZE pow(2, 12)
typedef enum {START, SEQ, IMP_STAND, IMP_REC, OMP_LOC_WS, OMP_GLB_WS, OMP_LOC_BB, OMP_GLB_BB, END} Mode;
//OMP_LOC_TB, OMP_GLB_TB, END} Mode;
typedef enum {RANDOM, STATIC, SYM, WORST} Test;
//Number generation
float random_float2 (int min, int max);
//Input Management
void inputParameters(int argc);
int valueInputed(int argc, const char* argv, int value);
int threadInit(const char* argv, int argc, int mode);
//Generation and deleting
float** createFloatSquareMatrix(int n);
float** createFloatSquareMatrixAligned(int n, int sublength);
float** allocateMatrixPerMode(Mode mode, int n, int sublength);
void initializeMatrix(float** M, Test test, int n);
void freeMemory(float** M, int size);
//Execution
bool executionProgram(float** M, float** T, Mode mode, int n, int sublength);
//Check Symmetry Algorithms
bool checkSym (float** M, int size);
bool checkSymImpStandard (float** M, int size, int sublength);
bool checkSymImpRecursive (float** M, int start_r, int end_r, int start_c, int end_c, int SUBLENGTH);
bool checkSymOMPLocal (float** M, int size);
bool checkSymOMPGlobal (float** M, int size);
//Transposition Algorithms
void matTranspose (float** M, float** T, int size);
void matTransposeImpStandard (float** M, float** T, int size, int sublength);
void matTransposeImpRecursive (float** M, float** T, int start_r, int end_r, int start_c, int end_c, int SUBLENGTH);
void matTransposeOMPWorkSharing (float** M, float** T,int size);
void matTransposeOMPBlockingBased (float** M, float** T, int size, int sublength);
//void matTransposeOMPTaskBased (float** M, float** T, int start_r, int end_r, int start_c, int end_c, int SUBLENGTH);
//Control Results
void printMatrix(float** M, int size);
void control(float** M, float** T, int N);
void bubbleSort(float* a, int size);
//Cache Management
void clearCache(long long int dimCache);
void clearAllCache(void);
//Files csv Management
float getSequential(const int dim, const int test);
void executeCommand(char command[]);
void openFile(const char* filename, const char* code, const int mode, const int dim, const int test, const int samples, const int num_threads, float avg_time, int type);
void openFilesAvgPerMode(const char* code, const int mode, const int n, const int test, const int samples, const int num_threads, const float avg_time);
void openFilesResultsPerMode(const char* code, const int mode, const int n, const int test, const int samples, const int num_threads, const float time);
#endif /* functions_h */
