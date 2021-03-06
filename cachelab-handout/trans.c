/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/*
 * func 0 (Transpose function optimized for 32x32 matrix): 
 * hits:1766, misses:287, evictions:255
*/
char transpose_32_desc[] = "Transpose function optimized for 32x32 matrix";
void transpose_32(int M, int N, int A[N][M], int B[M][N]) {
    int temp;
    int isDiagonalVal = 0;
    for (int i = 0; i <= N - 8; i += 8)
    {
        for (int j = 0; j <= M - 8; j += 8) {
            for (int ii = i; ii < i + 8; ii++) {
                for (int jj = j; jj < j + 8; jj++) {
                    //  避免在一次循环中读取A并写入B对角线上的元素
                    if (ii == jj)
                    {
                        isDiagonalVal = 1;
                        temp = A[ii][jj];
                    }
                    else {
                        B[jj][ii] = A[ii][jj];
                    }
                }
                if (isDiagonalVal)
                {
                    B[ii][ii] = temp;
                    isDiagonalVal = 0;
                }
            }
        }
    }
}
/*
 * func 0 (Transpose function optimized for 64x64 matrix): 
 * hits:6530, misses:1667, evictions:1635
*/
char transpose_64_desc[] = "Transpose function optimized for 64x64 matrix";
void transpose_64(int M, int N, int A[N][M], int B[M][N]) {
    int a0, a1, a2, a3, a4, a5, a6, a7;
    for (int i = 0; i <= N - 4; i += 4)
    {
        for (int j = 0; j <= M - 4; j += 4) {
            for (int ii = i; ii < i + 4; ii++) {
                for (int jj = j; jj < j + 4; jj++) {
                    //  避免在一次循环中读取A并写入B对角线上的元素
                    int imod = ii % 4;
                    int jmod = jj % 4;
                    if (imod == 0 && jmod == 0)
                    {
                        a0 = A[ii][jj];
                    }
                    else if (imod == 1 && jmod == 1)
                    {
                        a3 = A[ii][jj];
                    }
                    else if (imod == 1 && jmod == 0)
                    {
                        a2 = A[ii][jj];
                    }
                    else if (imod == 0 && jmod == 1)
                    {
                        a1 = A[ii][jj];
                    }
                    else if (imod == 2 && jmod == 2)
                    {
                        a4 = A[ii][jj];
                    }
                    else if (imod == 2 && jmod == 3)
                    {
                        a5 = A[ii][jj];
                    }
                    else if (imod == 3 && jmod == 2)
                    {
                        a6 = A[ii][jj];
                    }
                    else if (imod == 3 && jmod == 3)
                    {
                        a7 = A[ii][jj];
                    }
                    else {
                        B[jj][ii] = A[ii][jj];
                    }
                    if (imod == 1 && jmod == 3)
                    {
                        B[jj-2][ii] = a3;
                        B[jj-3][ii] = a2;
                        B[jj-2][ii-1] = a1;
                        B[jj-3][ii-1] = a0;
                    }
                    if (imod == 3 && jmod == 3) {
                        B[jj][ii] = a7;
                        B[jj-1][ii] = a6;
                        B[jj][ii-1] = a5;
                        B[jj-1][ii-1] = a4;
                    }
                    
                }
            }
        }
    }
}

/* 
 * 
 */
char trans_67_desc[] = "Transpose function optimized for 61x67 matrix";
void trans_67(int M, int N, int A[N][M], int B[M][N])
{
}

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    // registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 
    registerTransFunction(transpose_32, transpose_32_desc); 
    registerTransFunction(transpose_64, transpose_64_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

