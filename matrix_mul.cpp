// Header file for the matrix multiplication


#include <stdio.h>
#include <hls_stream.h> // axis stream header directory
#include <ap_int.h>    //declaring for the last bit
#define MATSIZE 8      // matrix max size
#define solution1      // assume having solution 1

/* define the data type for matrix input and output  */
# ifdef solution1
typedef float mat_dtype;
# endif

// define the axis data structure

struct axis_data {
	mat_dtype data;  // matrix data type
	ap_uint<1> last; // last signal data size of 1'b1
};

/* Redefine our function so that we do not to use at many places
 * it contain the structure name
 * input and output pins with their addresses (depend upon user)
 */

void matrix_multiplication_1 (hls :: stream<axis_data> &in_A, hls:: stream<axis_data> &out_C);
