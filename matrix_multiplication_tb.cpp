// Testbench for matrix_multiplication
#include "matrix_mul.h"
#include <stdio.h>
#include <math.h>

void matrix_multiplication_bm(double input_A [MATSIZE][MATSIZE], double input_B [MATSIZE][MATSIZE] , double output_C [MATSIZE][MATSIZE]);

// main function for test data

int main ()
{
	double input_A [MATSIZE][MATSIZE];
	double input_B [MATSIZE][MATSIZE];
	double output_C_BM[MATSIZE][MATSIZE] , output_C_HW[MATSIZE][MATSIZE];
	int row, column;

	//generate test data using loop1 (with random function or rand values)

	loop1 : for (row = 0; row < MATSIZE; row++)
	{
		loop2 : for (column = 0; column < MATSIZE; column++)
		{
               input_A[row][column] = rand()%100;
               input_B[row][column] = rand()%100 ;
		}
	}


	matrix_multiplication_bm (input_A, input_B, output_C_BM);

   // after this step we need to convert this to axis stream based signal

	axis_data local_stream;
	hls :: stream<axis_data> in_A,  out_C;  // b is optional



	loop_input_A1 : for (row = 0; row < MATSIZE; row++)
			{
				loop_input_A2 : for (column = 0; column < MATSIZE; column++)
				{
					local_stream.data = input_A[row][column] ;

					// generating the last signal and strobe signal

					if ((row == MATSIZE - 1) && (column == MATSIZE - 1 ))
						local_stream.last = 1;
					else
						local_stream.last = 0;
					 in_A.write(local_stream);
				}


			}

	// generate stream input for B input interface
	loop_input_B1 : for (row = 0; row < MATSIZE; row++)
			{
				loop_input_B2 : for (column = 0; column < MATSIZE; column++)
				{
					local_stream.data = input_B[row][column] ;

					// generating the last signal and strobe signal

					if ((row == MATSIZE - 1) && (column == MATSIZE - 1))
						local_stream.last = 1;
					else
						local_stream.last = 0;
					 in_A.write(local_stream);
				}


			}


	// depending upon which ip is active
	//calling if_def solution and axis_stream function

#ifdef solution1
	matrix_multiplication_1 ( in_A, out_C);
#endif
 // receive  stream input from the hardware interface for output_C
	loop_output_C1 : for (row = 0 ; row < MATSIZE ; row++ )
	{
		loop_output_C2: for (column = 0 ; column <MATSIZE; column++)
		{
			local_stream = out_C.read();
			output_C_HW [row][column] = local_stream.data;
		}

	}



	loop_compare_C1 : for (row = 0 ; row < MATSIZE ; row++)
	{
		loop_compare_C2 : for (column = 0 ; column < MATSIZE ; column++ )
		{
			if (fabs (output_C_HW[row][column] - output_C_BM[row][column])>0.01)
			{
				printf ("error at row index %d and error at column index %d \n",row,column );
				printf ("Hardware output  %f \n",output_C_HW[row][column] );
				printf ("Software output  %f \n",output_C_BM[row][column] ); // benchmark used as software
				return 1;

			}
		}
	}

	printf ("no error\n");
	return 0;

}

void matrix_multiplication_bm(double input_A [MATSIZE][MATSIZE], double input_B [MATSIZE][MATSIZE] , double output_C[MATSIZE][MATSIZE])
{
	// Matrix Multiplication
    int row , column, index;

	loop1 : for (row = 0; row < MATSIZE; row++)
	{
		loop2 : for (column = 0; column < MATSIZE; column++)
		{

			      double res = 0;

		loop3 :  for (index = 0; index < MATSIZE; index++)
		{
			for (index = 0 ; index < MATSIZE ; index++)
			{
				res += input_A[row][index] * input_B[index][column];
			}

			output_C[row][column] = res; // this output send on stream interface
		}
	}
}

}
