#include "matrix_mult.h"
#include <ac_assert.h>

//WIDTH = 6
//RESULT_WIDTH = 15
//
//- Using Memory Mapping:																|- register/wire based I/O interfaces:
//		Unrolled all loops - Pipeline Main = 8 - Block size = 2 - Default Word Width	|	Unrolled all loops - Pipeline Main = 8 
//		=> Throughput = 8, Area = 558335.06, Latency = 7								|	=> Throughput = 8, Area = 354439.82, Latency = 5								
//																						|			
//		Unrolled all loops - Pipeline Main = 8 - Block size = 4 - Default Word Width	|	
//		=> Throughput = 8, Area = 484031.03, Latency = 9								|	
//																						|		
//		Unrolled all loops - Pipeline Main = 8 - Block size = 8 - Default Word Width	|	
//		=> Throughput = 8, Area = 307354.22, Latency = 17								|			
//																						|	



void matrixMultHLS(hls_data_t A[N][N], hls_data_t B[N][N], hls_result_t C[N][N], int transpose)
{
    // Check that transpose has legal value
    assert(transpose >= 0 && transpose < 4);
    
    // Transpose both A and B before multiplication
    if (transpose == 3)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                hls_result_t C_temp = 0;
                for (int k = 0; k < N; k++)
                {
                    C_temp += A[k][i] * B[j][k];
                }
                C[i][j] = C_temp;
            }
        }            
    }
    // Transpose B before multiplication
    else if (transpose == 2)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                hls_result_t C_temp = 0;
                for (int k = 0; k < N; k++)
                {
                    C_temp += A[i][k] * B[j][k];
                }
                C[i][j] = C_temp;
            }
        }            
    }
    // Transpose A before multiplication
    else if (transpose == 1)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                hls_result_t C_temp = 0;
                for (int k = 0; k < N; k++)
                {
                    C_temp += A[k][i] * B[k][j];
                }
                C[i][j] = C_temp;
            }
        }            
    }
    // No transposition 
    else if (transpose == 0)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                hls_result_t C_temp = 0;
                for (int k = 0; k < N; k++)
                {
                    C_temp += A[i][k] * B[k][j];
                }
                C[i][j] = C_temp;
            }
        }            
    }
}
