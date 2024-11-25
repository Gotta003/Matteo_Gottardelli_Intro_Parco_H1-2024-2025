//
//  functions.c
//  Matrix Transposition
//
//  Created by Matteo Gottardelli on 16/11/24.
//

#include "functions.h"

/*
 * Name: random_float2
 * Generates a random float number in a range with 2 decimals
 * Input:
 *      min (int) - Minimum value of the range
 *      max (int) - Maximum value of the range
 * Output: float - Randomize value with 2 decimals
 */

float random_float2 (int min, int max) {
    return (rand()%(max-min+1)+min)*1.0/100;
}

/*
 * Name: createFloatSquareMatrix
 * Create in the heap a square matrix
 * Input: n (int) - Side dimension of the matrix
 * Output: float** - Matrix address, it's a heap array long n, having
 * in each position an heap array of float long n
 */

float** createFloatSquareMatrix(int n) {
    float** temp=(float**)malloc(sizeof(float*)*n);
    if(temp==NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    for (int i=0; i<n; i++) {
        temp[i]=(float*)malloc(sizeof(float)*n);
        if(temp[i]==NULL) {
            printf("Memory allocation failed\n");
            free(temp);
            exit(1);
        }
    }
    return temp;
}

/*
 * Name: createFloatSquareMatrixAligned
 * Create in the heap a square matrix, but aligned in memory according
 * to a length, we use a n/4, which is the minimum matrix that can be inputed
 * in this application. The variable requires bit length, so if we like subsets
 * of a particular size, we have to multiple the size of the type variable
 * Input: n (int) - Side dimension of the matrix
 * Output: float** - Matrix address, it's a heap array long n, having
 * in each position an heap array of float long n
 */
//For sake of example 16
float** createFloatSquareMatrixAligned(int n, int sublength) {
    float** temp=(float**)aligned_alloc(sublength*sizeof(float*), sizeof(float*)*n);
    if(temp==NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    for (int i=0; i<n; i++) {
        temp[i]=(float*)aligned_alloc(sublength*sizeof(float*), sizeof(float)*n);
        if(temp[i]==NULL) {
            printf("Memory allocation failed\n");
            free(temp);
            exit(1);
        }
    }
    return temp;
}

/*
 * Name: freeMemory
 * Free the memory allocated in heap, being careful of properly deallocating
 * array of pointers
 * Input:
 *      M (float**) - The allocated in heap matrix
 *      size (int) - Dimension of the side of the squared matrix
 * Output: none
 */

void freeMemory(float** M, int size) {
    if(M!=NULL) {
        for (int i=0; i<size; i++) {
            free(M[i]);
        }
        //free(M[0]);
        free(M);
    }
}

/*
 * Name: checkSym
 * Verifies if the square matrix in input is symmetric, so the matrix and its
 * transpose are equal (M[i][j]=M[j][i] - i, j counters of rows and columns
 * respectively)
 * Input:
 *      M (float**) - The allocated in heap matrix
 *      size (int) - Dimension of the side of the squared matrix
 * Output: bool (true - Matrix is symmetric; false - The matrix doesn't concide
 * with its transpose
 */
/*  j
  i - 0 1 2 .. N-2 N-1
    0
    1 x
    2 x x
   ..
  N-2 x x x ..
  N-1 x x x ..  x

 */
/* When one combination doesn't concide we can istantaneously
determine that the matrix is not symmetric */
bool checkSym (float** M, int size) {
    bool returnBool=true;
    for (int i=1; i<size && returnBool; i++) {
        for (int j=0; j<i && returnBool; j++) {
            if(M[i][j]!=M[j][i]) {
                returnBool=false;
            }
        }
    }
    return returnBool;
}

/*
 * Name: checkSymImpStandard
 * See checkSym -Allocating the matrix in a certain manner we will increase
 * spatial temporality
 * Input:
 *      M (float**) - The allocated in heap matrix
 *      size (int) - Dimension of the side of the squared matrix
        int sublength - Blocks of a submatrix
 * Output: bool (true - Matrix is symmetric; false - The matrix doesn't concide
 * with its transpose
 */
bool checkSymImpStandard (float** M, int size, int sublength) {
    bool returnBool=true;
    for (int i=1; i<size && returnBool; i+=sublength) {
        for (int j=0; j<i && returnBool; j+=sublength) {
            for (int k=i; k<i+sublength && returnBool; k++) {
                for (int l=j; l<j+sublength && returnBool; l++) {
                    if(M[k][l]!=M[l][k]) {
                        returnBool=false;
                    }
                }
            }
        }
    }
    return returnBool;
}
/*
 * Name: checkSymImpRecursive
 * See checkSym - This technique tries to arrive to the lowest matrix possible the 16x16 and from that operate recursively, dividing in these smaller matrix
 * Input:
 *      M (float**) - The allocated in heap matrix
 *      start_r (int) - Start row
 *      end_r (int) - End row
 *      start_c (int) - Start column
 *      end_c (int) - End column
 * Output: bool (true - Matrix is symmetric; false - The matrix doesn't concide
 * with its transpose
 */
bool checkSymImpRecursive (float** M, int start_r, int end_r, int start_c, int end_c, int SUBLENGTH) {
    int r=end_r-start_r;
    int c=end_c-start_c;
    if(r<=SUBLENGTH && c<=SUBLENGTH) {
        bool returnBool=true;
        for (int i=start_r; i<end_r && returnBool; i++) {
            int j_min=MIN(end_c, i);//Typically will always be i
            for (int j=start_c; j<j_min && returnBool; j++) {
                if (M[i][j]!=M[j][i]) {
                    returnBool=false;
                }
            }
        }
        return returnBool;
    }
    else {
        //start_c<=end_r TO ACHIEVE THIS WE REMOVE THE BOTTOMLEFT
        if(checkSymImpRecursive(M, start_r, start_r+r/2, start_c, start_c+c/2, SUBLENGTH)) {//UPPERLEFT
            if(checkSymImpRecursive(M, start_r+r/2, end_r, start_c, start_c+c/2, SUBLENGTH)) {//UPPERRIGHT
                //if(checkSymImpRecursive(M, start_r, start_r+r/2, start_c+c/2, end_c, SUBLENGTH)) {//BOTTOMLEFT start_c<=end_r
                return checkSymImpRecursive(M, start_r+r/2, end_r, start_c+c/2, end_c, SUBLENGTH);
                //}
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
}
/*
 * Name: checkSymOMPLocal
 * See checkSym - This technique tries to take the standard sequential function
 * and paralleling with OMP. The sequential code can return a variable, but that
 * can't be done in OMP because it has to reach a barrier and close the parallel
 * region. The algorithm via a local variable tries to cancel the for clause
 * when finding a not matchable. It writes the general variable and then close the
 * other threads, as soon as possible, exiting in advance from the cycle. We gain in
 * preventing unefficient cycles, but atomic (in order to avoid race conditions) and cancel
 * for will bring overhead
 * Input:
 *      M (float**) - The allocated in heap matrix
 *      size (int) - Dimension of the side of the squared matrix
 * Output: bool (true - Matrix is symmetric; false - The matrix doesn't concide
 * with its transpose
 */
bool checkSymOMPLocal (float** M, int size) {
    bool returnCondition=true;
    #pragma omp parallel
    {
        bool localBool=true;
        #pragma omp for schedule(static) //nowait
        for (int i=1; i<size; i++) {
            #pragma omp flush(returnCondition)
            #pragma omp atomic read
            localBool=returnCondition;
            if (!localBool){
                #pragma omp cancel for
                #pragma omp cancellation point for
            }
            else {
                for (int j=0; localBool && j<i; j++) {
                    if(M[i][j]!=M[j][i]) {
                        #pragma omp atomic write
                        returnCondition=false;
                        localBool=false;
                    }
                }
            }
        }
    }
    return returnCondition;
}

/*
 * Name: checkSymOMPGlobal
 * See checkSym - Is like the previous function, but this technique instead of trying to use a local
 * variable, uses the reduction clause and that the end does an "and logical" operation in order
 * that if a thread has met there at least a false that is displayed. But having a reduction, so a
 * nowait for parallel region, we can't use cancel for and we will have to do all the iteration.
 * Having reduction preserves us from atomic and the cancel, in order to reduce the iteration
 * in inner for a condition is inserted, but thanks to reduction, we handle possible race conditions
 * Input:
 *      M (float**) - The allocated in heap matrix
 *      size (int) - Dimension of the side of the squared matrix
 * Output: bool (true - Matrix is symmetric; false - The matrix doesn't concide
 * with its transpose
 */
bool checkSymOMPGlobal (float** M, int size) {
    bool returnCondition=true;
    #pragma omp parallel for schedule(static) reduction(&&:returnCondition) //nowait
    for(int i=1; i<size; i++) {
        if(returnCondition) {
            for(int j=0; returnCondition && j<i; j++) {
                if(M[i][j]!=M[j][i]) {
                    returnCondition=false;
                }
            }
        }
    }
    return returnCondition;
}
/*
 * Name: matTranspose
 * Does a standard transposition. Even if the one with a temporal variable is more
 * efficient considering the cycle will be on half of that matrix, this is a more
 * general one, which allows us to use more elements to optimize this code.
 * Input:
 *      M (float**) - The allocated in heap matrix
 *      size (int) - Dimension of the side of the squared matrix
 * Output: float** - Resulting matrix, after transposition
 */
void matTranspose (float** M, float** T, int size) {
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            T[j][i]=M[i][j];
        }
    }
}
/*
 * Name: matTransposeImpStandard
 * This code applies the block logic with an inputed subset, considering that we
 * allocated with SUBLENGTH constant, is recommended when calling this function to use
 * that variable. The logic is the same as checkSymImpStandard, but with all the elements.
 * I decided to not put an if excluding the elements on the main diagonal, because with high
 * number of matrix, there would have to be taken multiple comparisons
 * Input:
 *      M (float**) - The allocated in heap matrix (preferable with aligned one for best performance)
 *      size (int) - Dimension of the side of the squared matrix
 *      sublength (int) - Recommended to use the same value used in aligned matrix
 * Output: float** - Resulting matrix, after transposition
 */
void matTransposeImpStandard (float** M, float** T, int size, int sublength) {
    for (int i=0; i<size; i+=sublength) {
        for (int j=0; j<size; j+=sublength) {
            int min_k=MIN(size, i+sublength);
            int min_l=MIN(size, j+sublength);
            for (int k=i; k<min_k; k++) {
                for (int l=j; l<min_l; l+=MINIMUMSUBLENGTH) {
                    T[k][l+0]=M[l+0][k];
                    T[k][l+1]=M[l+1][k];
                    T[k][l+2]=M[l+2][k];
                    T[k][l+3]=M[l+3][k];
                    T[k][l+4]=M[l+4][k];
                    T[k][l+5]=M[l+5][k];
                    T[k][l+6]=M[l+6][k];
                    T[k][l+7]=M[l+7][k];
                }
            }
        }
    }
}
/*
 * Name: matTransposeImpRecursive
 * This code applies the recursive logic. The logic is the same as checkSymImpRecursive, but with
 * all the elements and instead of returning a boolean, it will always work populating the
 * destination allocated matrix. I decided to not put an if excluding the elements on the main
 * diagonal, because with high number of matrix, there would have to be taken multiple comparisons.
 * Input:
 *      M (float**) - The start allocated in heap matrix
 *      T (float**) - The destination allocated in heap matrix (has to be allocated outside)
 *      start_r (int) - Start row
 *      end_r (int) - End row
 *      start_c (int) - Start column
 *      end_c (int) - End column
 * Output: none
 */
void matTransposeImpRecursive (float** M, float** T, int start_r, int end_r, int start_c, int end_c, int SUBLENGTH) {
    int r=end_r-start_r;
    int c=end_c-start_c;
    if(r<=SUBLENGTH && c<=SUBLENGTH) {//This is because with the values it will be this value, otherwise there are problems in the code
        for (int i=start_r; i<end_r; i++) {
            for (int j=start_c; j<end_c; j+=MINIMUMSUBLENGTH) {
                T[i][j+0]=M[j+0][i];
                T[i][j+1]=M[j+1][i];
                T[i][j+2]=M[j+2][i];
                T[i][j+3]=M[j+3][i];
                T[i][j+4]=M[j+4][i];
                T[i][j+5]=M[j+5][i];
                T[i][j+6]=M[j+6][i];
                T[i][j+7]=M[j+7][i];
            }
        }
    }
    else {
        int mid_r=start_r+r/2;
        int mid_c=start_c+c/2;
        matTransposeImpRecursive(M, T, start_r, mid_r, start_c, mid_c, SUBLENGTH);//UPPERLEFT
        matTransposeImpRecursive(M, T, mid_r, end_r, start_c, mid_c, SUBLENGTH);//UPPERRIGHT
        matTransposeImpRecursive(M, T, start_r, mid_r, mid_c, end_c, SUBLENGTH);//BOTTOMLEFT
        matTransposeImpRecursive(M, T, mid_r, end_r, mid_c, end_c, SUBLENGTH);//BOTTOMRIGHT
    }
}
/*
 * Name: matTransposeOMPWorkSharing
 * This code applies the workSharing logic an possible implementation for OMP. It takes the sequential code and parallelize it. Having all the matrix to loop has previously explained
    was use collapse.
 *      M (float**) - The start allocated in heap matrix
 *      T (float**) - The destination allocated in heap matrix (has to be allocated outside)
 *      start_r (int) - Start row
 *      end_r (int) - End row
 *      start_c (int) - Start column
 *      end_c (int) - End column
 * Output: none
 */
void matTransposeOMPWorkSharing (float** M, float** T, int size) {
    #pragma omp parallel for collapse(2) schedule(static) shared(M, T)
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            T[i][j]=M[j][i];
        }
    }
}
//-fopt-info-vec
//export OMP_PROC_BIND=spread
//Because of booltenecks I tried to optimize it
void matTransposeOMPBlockingBased (float** M, float** T, int size, int sublength) {
    /*#ifdef _OPENMP
    int max_threads=MIN(size / sublength, omp_get_num_threads());
    omp_set_num_threads(max_threads);
    #endif*/
    #pragma omp parallel for collapse(2) schedule(static) shared(M, T)
    for (int i=0; i<size; i+=sublength) {
        for (int j=0; j<size; j+=sublength) {
            /*Used for small blocking and small n° threads*/
            //int smaller_block=sublength/2;
            int k_min=MIN(size, i+sublength);
            int l_min=MIN(size, j+sublength);
            for (int k=i; k<k_min; k++) {
                #pragma omp simd
                for (int l=j; l<l_min; l++) {
                    __builtin_prefetch(&M[l][k], 0, 1);
                    __builtin_prefetch(&T[k][l], 1, 1);
                    T[k][l]=M[l][k];
                }
            }
            
        }
    }
}

void printMatrix(float** M, int size) {
    for (int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            printf("%.2f\t", M[i][j]);
        }
        printf("\n");
    }
}
void control(float** M, float** T, int N) {
    printf("[%d][%d] M: %.2f T: %.2f\n", 1, 2, M[1][2], T[2][1]);
    printf("[%d][%d] M: %.2f T: %.2f\n", N/2-4, N/2+2, M[N/2-4][N/2+2], T[N/2+2][N/2-4]);
    printf("[%d][%d] M: %.2f T: %.2f\n", N-5, N-3, M[N-5][N-3], T[N-3][N-5]);
}
void clearCache(long long int dimCache) {
    float* cacheBuffer;
    if (posix_memalign((void**)&cacheBuffer, 64, dimCache) != 0) {
        fprintf(stderr, "Failed to allocate aligned memory\n");
        exit(1);
    }
    for (long long int i=0; i<dimCache/sizeof(float); i++) {
        cacheBuffer[i]=0.0f;
    }
    free(cacheBuffer);
}
void clearAllCache(void) {
    clearCache(CACHESIZEL1D);
    clearCache(CACHESIZEL1I);
    clearCache(CACHESIZEL2);
    clearCache(CACHESIZEL3);
}
float getSequential(const int dim, const int test) {
    char line[256];
    int findex, fmode, fdimension, ftest, fsamples, fthreads;
    char fcompile[20];
    float favg_time=0.0, fseq_time, fspeedup, fefficiency;
    FILE* file=fopen(FILENAMESEQ, "a+");
    if(file==NULL) {
        fprintf(stderr, "Couldn't open or create file %s\n", FILENAMESEQ);
        exit(1);
    }
    bool found=false;
    fgets(line, sizeof(line), file);
    while(!found && (fscanf(file, "%d %10s %d %d %d %d %d %f %f %f %f%%", &findex, fcompile, &fmode, &fdimension, &ftest,
          &fsamples, &fthreads, &favg_time, &fseq_time,
          &fspeedup, &fefficiency)==11)) {
        if(fmode==1 && (strcmp(fcompile, "SO0")==0) && fdimension==dim && ftest==test) {
            found=true;
        }
    }
    fclose(file);
    if(found) {
        return favg_time;
    }
    else {
        return 0.00;
       /* clearAllCache();
        char command[256];
        //snprintf(command, sizeof(command), "gcc -O0 functions.c transpose.c -o transpose -lm");
       // executeCommand(command);
        float get_integer=(log(dim)/log(2));
        snprintf(command, sizeof(command), "taskset -c 0 ./transpose SO0 1 %d %d %d", (int)get_integer, test, samples);
        executeCommand(command);
        return getSequential(dim, test, samples);*/
    }
}
void executeCommand(char command[]) {
    int result=system(command);
    if (result != 0) {
       fprintf(stderr, "Error executing ./transpose command\n");
       exit(1);
   }
}
void openFile(const char* filename, const char* code, const int mode, const int dim, const int test, const int samples, const int num_threads, float avg_time, int type) {
    FILE* file=fopen(filename, "a+"); //read and append
    if(file==NULL) {
        fprintf(stderr, "Couldn't open or create %s\n", filename);
        exit(1);
    }
    //int lines=countFile(file);
    FILE* wc=NULL;
    if(type==1) {
        wc=popen("wc -l < resultsGeneral.csv", "r");
    }
    else{
        if (type==0) {
            wc=popen("wc -l < timesGeneral.csv", "r");
        }
    }
    if (wc==NULL) {
        fprintf(stderr, "Error running wc -l\n");
        fclose(file);
        exit(1);
    }
    int lines;
    fscanf(wc, "%d", &lines);
    fclose(wc);
    if(lines==0){
        lines=1;
    }
    float seq_time = 0.0;
    float speedup=0.0;
    float efficiency=0.0;
    if(mode!=SEQ && type==1) {
        fclose(file);
        seq_time=getSequential(dim, test);
        file=fopen(filename, "a+"); //read and append
        if(file==NULL) {
            fprintf(stderr, "Couldn't open or create %s\n", filename);
            exit(1);
        }
        speedup=seq_time/avg_time;
        efficiency=speedup/num_threads*100;
    }
    else {
        seq_time=avg_time;
        speedup=seq_time/avg_time;
        efficiency=speedup/num_threads*100;
    }
    fseek(file, 0, SEEK_END);
    long long file_size=ftell(file);
    if(file_size==0 && type==1) {
        fprintf(file, "%-10s %-10s %-5s %-10s %-10s %-10s %-10s %-15s %-15s %-10s %-10s\n", "N°", "Compile", "Mode", "Dimension", "Test_Mode", "Samples", "Threads", "Avg_Time(s)", "Seq_time(s)", "Speedup", "Efficiency");
    }
    else {
        if(file_size==0 && type==0) {
            fprintf(file, "%-10s %-10s %-5s %-10s %-10s %-10s %-10s %-15s\n", "N°", "Compile", "Mode", "Dimension", "Test_Mode", "Samples", "Threads", "Time(s)");
        }
    }
    char void_element='-';
    if(type==0) {
        fprintf(file, "%-10d %-10s %-5d %-10d %-10d %-10d %-10d %-15.12f\n", lines, code, mode, dim, test, samples, num_threads, avg_time);
    }
    else {
        if(seq_time<1e-9) {
            fprintf(file, "%-10d %-10s %-5d %-10d %-10d %-10d %-10d %-15.12f %-15c %-10c %-10c%%\n", lines, code, mode, dim, test, samples, num_threads, avg_time, void_element, void_element, void_element);
        }
        else {
            fprintf(file, "%-10d %-10s %-5d %-10d %-10d %-10d %-10d %-15.12f %-15.12f %-10.2f %-10.2f%%\n", lines, code, mode, dim, test, samples, num_threads, avg_time, seq_time, speedup, efficiency);
        }
    }
    fclose(file);
}
void inputParameters(int argc) {
    if(argc!=6 && argc!=7) {
        fprintf(stderr, "Invalid parameters. They are:\n\n(1) Code identifying the compilation (everything is acceptable, default one is SO0, is used as reference for sequential code -O0 and will be used as comparison for speedup and efficiency)\n\n(2)Integer for mode. The modes are:\n1. Sequential Code\n2. Implicit Parallelism (Blocking)\n3. Implicit Parallelism (Recursive)\n4. Explicit Parallelism (OMP - SymLocalVar + Work-Sharing)\n5. Explicit Paralellelism (OMP - SymLocalVar + Block-Based)\n6. Explicit Parallelism (OMP - SymGeneralVar + Work-Sharing)\n7. Explicit Paralellelism (OMP - SymGeneralVar + Block-Based)\n\n(3) Exponential of 2 from 4 to 12 (16->4, 64->6, 1024->10, 4096->12)\n\n(4) Testing:\n0. Matrix is generated randomly\n1. A default test matrix is generated according to an algorithm (used for testing, with one interaction in checkSym and Transposition)\n2. A default test symmetric matrix (only checkSym works)\n3. The worst case scenario with a matrix, with only one element different not on main diagonal\n\n(5) Number of samples (>25)\n\n(6) If selected modes from 1 to 3 is not required, it will be ignored, but if are the others (OMP) it is important.\n\n");
        exit(1);
    }
}
int valueInputed(int argc, const char* argv, int value) {
    int returnValue=-1;
    if(value>=2 && value<=6) {
        switch (value) {
            case 2: {
               returnValue=atoi(argv);
                if (returnValue<=START || returnValue>=END) {
                    fprintf(stderr, "Invalid Mode entered.\nThe modes are:\n1. Sequential Code\n2. Implicit Parallelism\n3. Explicit Parallelism (OMP)\n\n");
                    exit(1);
                }
            }
                break;
            case 3: {
                returnValue=pow(2, atoi(argv));
                if (returnValue<MIN_SIZE || returnValue>MAX_SIZE) {
                    fprintf(stderr, "Invalid dimension, it has to be a power of 2 between 4 and 12\n\n");
                    exit(1);
                }
            }
                break;
            case 4: {
                returnValue=atoi(argv);
                if(returnValue>3 || returnValue<0) {
                    fprintf(stderr, "Invalid testing, it has to be 0 (random), 1 (standard test no symmetric), 2 (standard test symmetric)\n\n");
                    exit(1);
                }
            }
                break;
            case 5: {
                returnValue=atoi(argv);
                if(returnValue<25) {
                    fprintf(stderr, "Invalid samples number, it has to at least of 25\n\n");
                    exit(1);
                }
            }
                break;
            case 6: {
                returnValue=atoi(argv);
                if (returnValue<=0 && argc==7) {
                    fprintf(stderr, "Invalid number inserted, it has to be positive\n\n");
                    exit(1);
                }
                else {
                    if (argc!=7) {
                        fprintf(stderr, "Invalid parameters. They are:\n\n(1) Code identifying the compilation (everything is acceptable, default one is SO0, is used as reference for sequential code -O0 and will be used as comparison for speedup and efficiency)\n\n(2)Integer for mode. The modes are:\n1. Sequential Code\n2. Implicit Parallelism (Blocking)\n3. Implicit Parallelism (Recursive)\n4. Explicit Parallelism (OMP - SymLocalVar + Work-Sharing)\n5. Explicit Paralellelism (OMP - SymLocalVar + Block-Based)\n6. Explicit Parallelism (OMP - SymGeneralVar + Work-Sharing)\n7. Explicit Paralellelism (OMP - SymGeneralVar + Block-Based)\n\n(3) Exponential of 2 from 4 to 12 (16->4, 64->6, 1024->10, 4096->12)\n\n(4) Testing:\n0. Matrix is generated randomly\n1. A default test matrix is generated according to an algorithm (used for testing, with one interaction in checkSym and Transposition)\n2. A default test symmetric matrix (only checkSym works)\n3. The worst case scenario with a matrix, with only one element different not on main diagonal\n\n(5) Number of samples (>25)\n\n(6) If selected modes from 1 to 3 is not required, it will be ignored, but if are the others (OMP) it is important.\n\n");
                        exit(1);
                    }
                }
            }
                break;
            default:
                exit(1);
                break;
        }
    }
    return returnValue;
}
int threadInit(const char* argv, int argc, int mode) {
    int NUM_THREADS;
    if(mode>=OMP_LOC_WS && mode<END){
        #ifdef _OPENMP
            NUM_THREADS=valueInputed(argc, argv, 6);
            printf("%d\n", NUM_THREADS);
            omp_set_num_threads(NUM_THREADS);
            #pragma omp parallel
            {
                printf("Thread %d out of %d threads\n", omp_get_thread_num(), omp_get_num_threads());
            }
            printf("\n");
        #else
            fprintf(stderr,"Forgot -fopenmp flag for MODE=%d, please to run in this mode insert that flag\n\n", mode);
            exit(1);
        #endif
    }
    else {
        #ifdef _OPENMP
                NUM_THREADS=1;
        #else
            fprintf(stderr,"Forgot -fopenmp flag for MODE=%d, please to run in this mode insert that flag\n\n", mode);
            exit(1);
        #endif
    }
    return NUM_THREADS;
}
float** allocateMatrixPerMode(Mode mode, int n, int sublength) {
    float** M=NULL;
    switch (mode) {
        case SEQ: {
            M=createFloatSquareMatrix(n);
        }
            break;
        case IMP_STAND:
        case IMP_REC:
        case OMP_LOC_WS:
        case OMP_GLB_WS:
        case OMP_LOC_BB:
        case OMP_GLB_BB:
        //case OMP_LOC_TB:
        //case OMP_GLB_TB:
        {
            M=createFloatSquareMatrixAligned(n, sublength);
        }
        break;
        case START:
        case END:
            exit(1);
            break;
        default:
            exit(1);
            break;
    }
    return M;
}
void initializeMatrix(float** M, Test test, int n) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            switch (test) {
                case RANDOM: M[i][j]=random_float2(0, 9999); break;
                case STATIC: M[i][j]=i*1.0/200000*n+j*1.0/100+1; break;
                case SYM:
                case WORST: M[i][j]=5.0; break;
                default: fprintf(stderr, "Unexpected Error\n"); exit(1);
            }
        }
    }
    if(test==WORST) {
        M[n-2][n-1]=3.0;
    }
}
bool executionProgram(float** M, float** T, Mode mode, int n, int sublength) {
    switch (mode) {
        case SEQ: {
            if(!checkSym(M, n)) {
                matTranspose(M, T, n);
                return false;
            }
        }
        break;
        case IMP_STAND: {
            if(!checkSymImpStandard(M, n, sublength)) {
                matTransposeImpStandard(M, T, n, sublength);
                return false;
            }
        }
        break;
        case IMP_REC: {
            if(!checkSymImpRecursive(M, 0, n, 0, n, sublength)) {
                matTransposeImpRecursive(M, T, 0, n, 0, n, sublength);
                return false;
            }
        }
        break;
        // OMP variables can be seen via OMP_DISPLAY_ENV=TRUE
        //In order to optimize it we use the following environment variables:
        /*
         * export OMP_NUM_THREADS=8          //Threads executing
         * export OMP_SCHEDULE=static    //Dynamic schedule
         * export OMP_PROC_BIND=TRUE         //Bind threads for cache locality
         * export OMP_PLACES=cores           //Thread placement strategy
         * export OMP_STACKSIZE=128M         //Maximum Matrix Dimension*2
         * export OMP_WAIT_POLICY=ACTIVE     //Reduce thread idle time
         * export OMP_NESTED=TRUE            //Enable nested parallelism for tasks
         * export OMP_MAX_ACTIVE_LEVELS=OMP_NUM_THREADS    //Allow two levels of parallelism
         * export OMP_CANCELLATION=TRUE      // Allow task cancellation
         */
        // WORK SHARING
        case OMP_LOC_WS: {
            #ifdef _OPENMP
            if(!checkSymOMPLocal(M, n)) {
                matTransposeOMPWorkSharing(M, T, n);
                return false;
            }
            #endif
        }
        break;
        case OMP_GLB_WS: {
            #ifdef _OPENMP
            if(!checkSymOMPGlobal(M, n)) {
                matTransposeOMPWorkSharing(M, T, n);
                return false;
            }
            #endif
        }
        break;
        //FOR BLOCK BASED HAVE SIMD -O3 -march=native -ffast-math -funroll-loops
        // -fopenmp -fopenmp-simd
        // • export OMP_PROC_BIND=true
        // • export OMP_PLACES=cores
        // • OMP_DYNAMIC=TRUE
        case OMP_LOC_BB: {
            #ifdef _OPENMP
            if(!checkSymOMPLocal(M, n)) {
                matTransposeOMPBlockingBased(M, T, n, sublength);
                return false;
            }
            #endif
        }
        break;
        case OMP_GLB_BB: {
            #ifdef _OPENMP
            if(!checkSymOMPGlobal(M, n)) {
                matTransposeOMPBlockingBased(M, T, n, sublength);
                return false;
            }
            #endif
        }
        break;
        //FOR TASK BASED WE ACTIVATE compile -O3 -march=native
        // • export OMP_DYNAMIC=TRUE
        // • export OMP_NESTED=TRUE
        // • export OMP_MAX_ACTIVE_LEVELS=2
        // • export OMP_PROC_BIND=spread
        /*case OMP_LOC_TB: {
            #ifdef _OPENMP
            omp_set_nested(1);
            if(!checkSymOMPLocal(M, n)) {
                #pragma omp parallel
                {
                    #pragma omp single
                    {
                        matTransposeOMPTaskBased(M, T, 0, n, 0, n, sublength);
                    }*/
                    /*int start=(n/(omp_get_num_threads()*2))*omp_get_thread_num();
                    int end=(n/(omp_get_num_threads()*2))*(omp_get_thread_num()+1);
                    matTransposeOMPTaskBased(M, T, start, end, start, end, sublength);
                }
                return false;
            }
            #endif
        }
        break;*/
        /*case OMP_GLB_TB: {
            #ifdef _OPENMP
            if(!checkSymOMPGlobal(M, n)) {
                #pragma omp parallel
                {
                    #pragma omp single
                    matTransposeOMPTaskBased(M, T, 0, n, 0, n, sublength);*/
                   /* int start=(n/(omp_get_num_threads()*2))*omp_get_thread_num();
                    int end=(n/(omp_get_num_threads()*2))*(omp_get_thread_num()+1);
                    matTransposeOMPTaskBased(M, T, start, end, start, end, sublength);
                }
                return false;
            }
            #endif
        }
        break;*/
        default:
            exit(1);
            break;
    }
    freeMemory(T, n);
    T=M;
    return true;
}
void openFilesAvgPerMode(const char* code, const int mode, const int n, const int test, const int samples, const int num_threads, const float avg_time) {
    openFile(FILENAMEGEN, code, mode, n, test, samples, num_threads, avg_time, 1);
    switch (mode) {
        case SEQ:
            openFile(FILENAMESEQ, code, mode, n, test, samples, num_threads, avg_time, 1);
            break;
        case IMP_STAND:
            openFile(FILENAMEIMPSTAND, code, mode, n, test, samples, num_threads, avg_time, 1);
            break;
        case IMP_REC:
            openFile(FILENAMEIMPREC, code, mode, n, test, samples, num_threads, avg_time, 1);
            break;
        case OMP_LOC_WS:
            openFile(FILENAMEOMPLOCWS, code, mode, n, test, samples, num_threads, avg_time, 1);
            break;
        case OMP_GLB_WS:
            openFile(FILENAMEOMPGLBWS, code, mode, n, test, samples, num_threads, avg_time, 1);
            break;
        case OMP_LOC_BB:
            openFile(FILENAMEOMPLOCBB, code, mode, n, test, samples, num_threads, avg_time, 1);
            break;
        case OMP_GLB_BB:
            openFile(FILENAMEOMPGLBBB, code, mode, n, test, samples, num_threads, avg_time, 1);
            break;
        /*case OMP_LOC_TB:
            openFile(FILENAMEOMPLOCTB, code, mode, n, test, samples, num_threads, avg_time, 1);
            break;
        case OMP_GLB_TB:
            openFile(FILENAMEOMPGLBTB, code, mode, n, test, samples, num_threads, avg_time, 1);
            break;*/
        default:
            exit(1);
            break;
    }
}
void openFilesResultsPerMode(const char* code, const int mode, const int n, const int test, const int samples, const int num_threads, const float time) {
    openFile(FILENAMETGEN, code, mode, n, test, samples, num_threads, time, 0);
    switch (mode) {
        case SEQ:
            openFile(FILENAMETSEQ, code, mode, n, test, samples, num_threads, time, 0);
            break;
        case IMP_STAND:
            openFile(FILENAMETIMPSTAND, code, mode, n, test, samples, num_threads, time, 0);
            break;
        case IMP_REC:
            openFile(FILENAMETIMPREC, code, mode, n, test, samples, num_threads, time, 0);
            break;
        case OMP_LOC_WS:
            openFile(FILENAMETOMPLOCWS, code, mode, n, test, samples, num_threads, time, 0);
            break;
        case OMP_GLB_WS:
            openFile(FILENAMETOMPGLBWS, code, mode, n, test, samples, num_threads, time, 0);
            break;
        case OMP_LOC_BB:
            openFile(FILENAMETOMPLOCBB, code, mode, n, test, samples, num_threads, time, 0);
            break;
        case OMP_GLB_BB:
            openFile(FILENAMETOMPGLBBB, code, mode, n, test, samples, num_threads, time, 0);
            break;
        /*case OMP_LOC_TB:
            openFile(FILENAMETOMPLOCTB, code, mode, n, test, samples, num_threads, time, 0);
            break;
        case OMP_GLB_TB:
            openFile(FILENAMETOMPGLBTB, code, mode, n, test, samples, num_threads, time, 0);
            break;*/
        default:
            exit(1);
            break;
    }
}
void bubbleSort(float* a, int size) {
    for (int i=0; i<(size-1); i++) {
        for (int j=0; j<(size-1-i); j++) {
            if(a[j]>a[j+1]) {
                float temp=a[j+1];
                a[j+1]=a[j];
                a[j]=temp;
            }
        }
    }
}