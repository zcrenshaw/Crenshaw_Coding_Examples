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

/* goes in squares*/
char squares_desc[] = "goes in squares";
void squares(int M, int N, int A[N][M], int B[M][N])
{
	int i,j,row,col,tmp, visited,blocksize;
	
	visited = 0;
	blocksize = 1;
	if (M == 32 && N == 32)
		blocksize = 8;
	if (M == 64 && N == 64)
		blocksize = 4;
	if (M == 32 && N == 64)
		blocksize = 4;

	for (row = 0; row < N; row += blocksize){
		for (col = 0; col < M; col+= blocksize){
			for (i = row; i < row + blocksize; i++) {
				for (j = col; j < col + blocksize; j++) {
					if (i != j) //not a diagonal
						B[j][i] = A[i][j];
					else {
						tmp = A[i][j];
						visited = 1; //marks that this diagonal has been visited
					}
				}
				if(visited) {
					B[i][i] = tmp;
					visited = 0;
				}
			}
		}
	}
}
/*
	int i,j;
	for (i = 0; i < N; i+=2) {
		for (j = 0; j < M; j+=2) {
			B[j][i] = A[i][j];
			B[j][i+1] = A[i+1][j];
			B[j+1][i] = A[i][j+1];
			B[j+1][i+1] = A[i+1][j+1];
		}
	}
*/
/* quick*/
char fast_boi_desc[] = "fast";
void fast_boi(int M, int N, int A[N][M], int B[M][N])
{
	int i, j,i1,j1,b,tmp;
	b = 1;
	for (i = 0; i < N; i+=b) {
		for (j = 0; j < M; j+=b) {
			for (i1 = i; i1 < i+b; i++) {
					for (j1 = j; j1 < j+b; j++) {
						tmp = A[i1][j1];
						B[j1][i1] = tmp;
					}
			}
		}
	}	
}

char new_desc[] = "new";
void new(int M, int N, int A[N][M], int B[M][N])
{
	int i,j,row,col,tmp, visited,blocksize;
	
	visited = 0;
	blocksize = 1;
	if (M == 32 && N == 32)
		blocksize = 8;
	if (M == 64 && N == 64)
		blocksize = 4;
	if (M == 32 && N == 64)
		blocksize = 4;

	for (row = 0; row < N; row += blocksize){
		for (col = 0; col < M; col+= blocksize){
			for (i = row; i < row + blocksize; i++) {
				for (j = col; j < col + blocksize; j++) {
					if (i != j) //not a diagonal
						B[j][i] = A[i][j];
					else {
						tmp = A[i][j];
						visited = 1; //marks that this diagonal has been visited
					}
				}
				if(visited) {
					B[i][i] = tmp;
					visited = 0;
				}
			}
		}
	}
}
				
/*
void mmm(double *a, double *b, double *c, int n) {
	int i, j, k;
	for (i = 0; i < n; i+=B)
		for (j = 0; j < n; j+=B)
			for (k = 0; k < n; k+=B)
				// B x B mini matrix multiplications 
				for (i1 = i; i1 < i+B; i++)
					for (j1 = j; j1 < j+B; j++)
						for (k1 = k; k1 < k+B; k++)
							c[i1*n+j1] += a[i1*n + k1]*b[k1*n + j1];
}
*/
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
	int i,j,row,col,tmp, visited,blocksize;
	
	visited = 0;
	blocksize = 1;
	if (M == 32 && N == 32)
		blocksize = 8;
	if (M == 64 && N == 64)
		blocksize = 4;
	if (M == 32 && N == 64)
		blocksize = 4;

	for (row = 0; row < N; row += blocksize){
		for (col = 0; col < M; col+= blocksize){
			for (i = row; i < row + blocksize; i++) {
				for (j = col; j < col + blocksize; j++) {
					if (i != j) //not a diagonal
						B[j][i] = A[i][j];
					else {
						tmp = A[i][j];
						visited = 1; //marks that this diagonal has been visited
					}
				}
				if(visited) {
					B[i][i] = tmp;
					visited = 0;
				}
			}
		}
	}
}


/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

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
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

    /* register jawn*/
	registerTransFunction(new, new_desc);

	registerTransFunction(squares,squares_desc);
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

