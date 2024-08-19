/// code for matrix multiplication :

#include "matrix_mul.h"   // including the matrix_mul header file
#ifdef solution1

void matrix_multiplication_1 (hls :: stream<axis_data> &in_A, hls:: stream<axis_data> &out_C)
{

	// adding interfaces through the directives
     #pragma HLS INTERFACE mode=ap_ctrl_none port=return // this will not introduce t_ready or t_valid control signals
     #pragma HLS INTERFACE mode=axis register_mode=both port=in_A
     #pragma HLS INTERFACE mode=axis register_mode=both port=out_C

	// define the local input and output variable for matrix

	mat_dtype input_A [MATSIZE][MATSIZE];
	mat_dtype input_B [MATSIZE][MATSIZE];

	mat_dtype output_C [MATSIZE][MATSIZE]; // output

	int row,  column , index;  // variable that are used in for loop
	axis_data local_stream; // to receive the data over one interface

	/* saving the data and storing into respective interface
	 * using loop
	 */

	loop_input_A1 : for (row = 0 ; row < MATSIZE ; row++)
	{
		loop_input_A2 :  for(column = 0 ; column < MATSIZE; column++)
		{
			local_stream = in_A.read();
			input_A[row][column]=local_stream.data;
		}

	}

	/* saving the data and storing into respective interface
	 * using loop
	 */

	loop_input_B1 : for (row = 0 ; row < MATSIZE ; row++ )
	{
		loop_input_B2 : for (column = 0 ; column <MATSIZE; column++)
		{
			local_stream = in_A.read();
			input_B[row][column] = local_stream.data;
		}

	}

	// Matrix Multiplication

	loop1 : for (row = 0; row < MATSIZE; row++)
	{
		loop2 : for (column = 0; column < MATSIZE; column++)
		{
			mat_dtype res = 0;
			for (index = 0 ; index < MATSIZE ; index++)
			{
				res += input_A[row][index] * input_B[column][index];
			}

			output_C[row][column] = res; // this output send on stream interface
		}
	}


	// stream output data back

	loop_output_C1 : for (row = 0; row < MATSIZE; row++)
		{
			loop_output_C2 : for (column = 0; column < MATSIZE; column++)
			{
				local_stream.data = output_C[row][column] ;

				// generating the last signal and strobe signal

				if ((row == MATSIZE - 1) && (column == MATSIZE - 1))
					local_stream.last = 1;
				else
					local_stream.last = 0;
				 out_C.write(local_stream);
			}


		}

}

# endif
