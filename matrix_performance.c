/*
 * This sample program is to establish the improvement in performance 
 * with cpu caches.
 *                      ________________
 *            -------->|   Main Memory  |
 *           |         |________________|
 *           |
 *           |
 *      ===============================================
 *           ^          
 *           |   _____________        ___________
 *           -->|  Cpu Cache |<----->|  Cpu Core |  
 *              |____________|       |___________|
 *
 *                Cache layout in a CPU
 *                =====================
 *
 * Here, we have two functions matrix_row_wise_read and matrix_column_wise_read.
 * matrix_row_wise_read:    Reads all columns in a row and go to next row.
 * matrix_column_wise_read: Reads all rows in a column and go to next column.
 *
 * matrix_row_wise_read exhibits spatial locality in the cpu cache more than
 * matrix_column_wise_read. This results in the performance difference between
 * row wise read and column wise read.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned int u_int;
typedef u_int 		 matrix_t;

void
matrix_row_wise_read (matrix_t     **matrix, u_int n_rows, u_int n_columns)
{
	u_int	row, column, value;

	/*
	 * Row wise read:
	 * Read all columns in first row and then go to second row and on and on.
	 */
	for (row = 0; row < n_rows; row++) {
		for (column = 0; column < n_columns; column++) {
			value = matrix[row][column];
		}
	}
}
 
void
matrix_column_wise_read (matrix_t     **matrix, u_int n_rows, u_int n_columns)
{
	u_int	row, column, value;

	/*
	 * Column wise read:
	 * Read all rows in first column and then go to second column and on and on.
	 */
	for (column = 0; column < n_rows; column++) {
		for (row = 0; row < n_columns; row++) {
			value = matrix[row][column];
		}
	}
}

void
main (u_int	argc, char *argv[])
{
	u_int 	   row, column, value, n_rows, n_columns;
	matrix_t **matrix;
	clock_t	   t;
	double     time_taken;

	if (argc != 3) {
		printf("Unexpected number of rows\n");
		return;
	}

	value     = 0;
	n_rows 	  = atoi(argv[1]);
	n_columns = atoi(argv[2]);

	printf("rows=%d columns=%d\n", n_rows, n_columns);

	matrix = calloc(n_rows, sizeof(u_int *));
	printf("Main ptr: %p\n", matrix);
	for (column = 0; column<n_columns; column++) {
		*(matrix+column) = calloc(n_columns, sizeof(u_int));
	}

	for (row = 0; row < n_rows; row++) {
		for (column = 0; column < n_columns; column++) {
			matrix[row][column] = value++;
		}
	}

	t = clock();
	matrix_column_wise_read(matrix, n_rows, n_columns);
	t = clock() - t;

	time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
	printf("Column wise read time: %f\n", time_taken);	

	t = clock();
	matrix_row_wise_read(matrix, n_rows, n_columns);
	t = clock() - t;

	time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
	printf("Row wise read time: %f\n", time_taken);	
}

