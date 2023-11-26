#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reallocate(int ****mat, int ***dim, int nr)
{
	*mat = realloc(*mat, nr * sizeof(int **));
	*dim = realloc(*dim, nr * sizeof(int *));
}

int **alloc_mat(int n, int m)
{
	int **matrix = (int **)malloc(n * sizeof(int *));
	//check dynamic memory allocation
	if (!matrix)
		return NULL;
	for (int i = 0; i < n; i++) {
		matrix[i] = (int *)malloc(m * sizeof(int));
		if (!matrix[i]) {
			return NULL;
			free(matrix[i]);
			}
	}
	return matrix;
}

void dim_alloc_mat(int **dim, int n)
{
	dim[n] = (int *)malloc(2 * sizeof(int));
}

void free_matrix(int rows, int **matrix)
{
	for (int i = 0; i < rows; i++)
		free(matrix[i]);
	free(matrix);
}

void remove_matrix(int ***mat, int **dim, int *nr)
{
	int index;
	//read the indices of the matrices to be removed
	scanf("%d", &index);
	if (index < *nr && index >= 0) {
		for (int i = 0; i < dim[index][0]; i++) {
			//delete matrix lines
			free(mat[index][i]);
		}
		free(mat[index]);
		//delete dimmensions
		free(dim[index]);
		for (int a = index; a < *nr - 1; a++) {
			//move the remaining matrices one position to the left
			mat[a] = mat[a + 1];
			//move dimensions to correspond to the matrixes
			dim[a] = dim[a + 1];
		}
		*nr = *nr - 1;
	} else {
		//haven't read enough matrices
		printf("No matrix with the given index\n");
	}
}

int overflow(int x)
{
	//function that solves overflow problems as required
	x %= 10007;
	if (x < 0)
		return x + 10007;
	else
		return x;
}

void dimensions(int **dim, int nr)
{
	int index = 0;
	//read the index
	scanf("%d", &index);
	//check if there is a matrix with the given index
	if (index < nr && index >= 0) {
		//access dimensions saved in dim
		printf("%d %d\n", dim[index][0], dim[index][1]);
	} else {
		//haven't read enough matrices
		printf("No matrix with the given index\n");
	}
}

void display(int ***mat, int **dim, int *nr)
{
	int index;
	//read the index
	scanf("%d", &index);
	//check if there is a matrix with the given index
	if (index < *nr && index >= 0) {
		//initialize n and m with the dimensions of the matrix to display
		int i = 0, j = 0, n = dim[index][0], m = dim[index][1];
		//using the array of matrices display the elements of the matrix
			for (i = 0; i < n; i++) {
				for (j = 0; j < m; j++)
					printf("%d ", mat[index][i][j]);
				printf("\n");
			}
	} else {
		//haven't read enough matrices
		printf("No matrix with the given index\n");
	}
}

void resize(int ***mat, int **dim, int nr)
{
	int index, columns, rows, i, j;
	scanf("%d", &index);
	scanf("%d", &rows);
	//array of rows used in resizeing
	int *rrs = (int *)malloc(rows * sizeof(int));
	//read rows indexes
	for (i = 0; i < rows; i++)
		scanf("%d", &rrs[i]);

	scanf("%d", &columns);
	//array of columns used in resizeing
	int *crs = (int *)malloc(columns * sizeof(int));
	//read columns indexes
	for (i = 0; i < columns; i++)
		scanf("%d", &crs[i]);
	if (index < nr && index >= 0) {
		//build the new matrix
		int **new_mat = alloc_mat(rows, columns);

		//add elemnts to the new matrix
		for (i = 0; i < rows; i++)
			for (j = 0; j < columns; j++)
				new_mat[i][j] = mat[index][rrs[i]][crs[j]];

		//free initial matrix memory
		free_matrix(dim[index][0], mat[index]);

		//modify matrix dimmensions in dim
		dim[index][0] = rows;
		dim[index][1] = columns;
		//replace the original matrix with the resized matrix
		mat[index] = new_mat;
	} else {
		//haven't read enough matrices
		printf("No matrix with the given index\n");
	}
	free(rrs);
	free(crs);
}

int sum(int ***mat, int **dim, int index)
{
	int i, j, sum = 0;
	//initialize the sum with 0 and add elements of the matrix given by index
	for (i = 0; i < dim[index][0]; i++)
		for (j = 0; j < dim[index][1]; j++) {
			sum += mat[index][i][j];
			sum = overflow(sum);
		}
	return sum;
}

void order(int ***mat, int **dim, int nr)
{
	int i, j;
	for (i = 0; i < nr - 1; i++)
		for (j = i + 1; j < nr; j++)
			if (sum(mat, dim, i) > sum(mat, dim, j)) {
				//interchange the addresses of matrices
				//create auxiliar matrix
				int **aux;

				//copy mat[i] in aux
				aux = mat[i];
				//save it's dimmensions
				int n = dim[i][0], m = dim[i][1];

				//put mat[j] in mat[i]
				mat[i] = mat[j];
				//save new dimmensions in dim
				dim[i][0] = dim[j][0];
				dim[i][1] = dim[j][1];

				//put aux in mat[j]
				mat[j] = aux;
				//save new dimmensions in dim
				dim[j][0] = n;
				dim[j][1] = m;
			}
}

void transp(int ***mat, int **dim, int nr)
{
	int index, i, j;
	scanf("%d", &index);
	if (index < nr && index >= 0) {
		//create new matrix in oreder to buildvthe transposed
		int **mat_trans = alloc_mat(dim[index][1], dim[index][0]);

		//save in the columns of mat_trans he lines of mat[index]
		for (i = 0; i < dim[index][1]; i++)
			for (j = 0; j < dim[index][0]; j++)
				mat_trans[i][j] = mat[index][j][i];

		//free initial matrix
		free_matrix(dim[index][0], mat[index]);
		//modify the dimmensions in dim
		int aux = dim[index][0];
		dim[index][0] = dim[index][1];
		dim[index][1] = aux;

		//replace the original matrix with the transposed matrix
		mat[index] = mat_trans;

	} else {
		printf("No matrix with the given index\n");
	}
}

int **multiply(int ***mat, int **dim, int ind1, int ind2)
{
	//function used to multiply 2 matrices given as parameter by indexces
	 int i, j, k;
	//alloc memory for new matrix
	//multy_mat will save the matrix resulted from multiplying
	int **multy_mat = alloc_mat(dim[ind1][0], dim[ind2][1]);

	//initiate elements with 0
	for (i = 0; i < dim[ind1][0]; i++)
		for (j = 0; j < dim[ind2][1]; j++)
			multy_mat[i][j] = 0;

	//multiply the matrices and save each product element in the new matrix
	for (i = 0; i < dim[ind1][0]; i++)
		for (j = 0; j < dim[ind2][1]; j++)
			for (k = 0; k < dim[ind1][1]; k++)
				multy_mat[i][j] += overflow(mat[ind1][i][k] * mat[ind2][k][j]);

	//solve once again overflow problems
	for (i = 0; i < dim[ind1][0]; i++)
		for (j = 0; j < dim[ind2][1]; j++)
			multy_mat[i][j] = overflow(multy_mat[i][j]);

	return multy_mat;
}

void multiply_square_mat(int **mat1, int **mat2, int n, int ***mat_rez)
{
	//multiply 2 square matrices with the same dimmmensions
	//the resulting matrix will be saved in mat_rez given as parameter
	int i, j, k;
	//alloc memory the new matrix that will result from multiplying
	int **product_mat = alloc_mat(n, n);

	//initiate elements with 0
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				product_mat[i][j] = 0;

	//multiply the matrices and save each product element in the new matrix
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				for (k = 0; k < n; k++)
					product_mat[i][j] += overflow(mat1[i][k] * mat2[k][j]);

		//solve once again overflow problems
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				product_mat[i][j] = overflow(product_mat[i][j]);

		//free the matrix where we want to save the new matrix
		free_matrix(n, *mat_rez);
		//add matrix
		*mat_rez =  product_mat;
}

int **power_up(int **matrix, int dim_mat, int power)
{
	//raise the matrix to the given power

	//alloc memory for the powerd up matrix
	int **mat_pwr = alloc_mat(dim_mat, dim_mat);

	//initialize the matrix as the neutral matrix in multiplication
	for (int i = 0; i < dim_mat; i++)
		for (int j = 0; j < dim_mat; j++)
			if (i == j)
				mat_pwr[i][j] = 1;
			else
				mat_pwr[i][j] = 0;

	//as long as the power is greater than 0, perform the following operations
	while (power) {
		if (power % 2 == 1)
			multiply_square_mat(mat_pwr, matrix, dim_mat, &mat_pwr);
		multiply_square_mat(matrix, matrix, dim_mat, &matrix);
		power /= 2;
	}
	free_matrix(dim_mat, matrix);
	//return matrix raised to power
	return mat_pwr;
}

void add_power(int ****mat, int **dim, int nr)
{
	/*function used to modify the original array matrix
	with the array raised to power*/
	int index, pwr;
	scanf("%d %d", &index, &pwr);

	//verify if the matrix with the given index exists
	if (index < nr && index >= 0) {
		//veryfy if the power is pozitive
		if (pwr >= 0) {
			//verify if the matrix is square
			if (dim[index][0] == dim[index][1]) {
				//create new matrix with the powerd up matrix
				int **mat_power = power_up((*mat)[index], dim[index][0], pwr);

				//add the modified matrix in the array
				(*mat)[index] = mat_power;
			} else {
				printf("Cannot perform matrix multiplication\n");
			}
		} else {
			printf("Power should be positive\n");
			}
	} else {
		printf("No matrix with the given index\n");
		}
}

void add_multiplied_mat(int ****mat, int ***dim, int *nr)
{
	int index1, index2;
	//read the indices of the matrices to be multiplied
	scanf("%d %d", &index1, &index2);

	//check if there are both matrices in memory
	if (index1 < *nr && index2 < *nr && index1 >= 0 && index2 >= 0) {
		//verify if matrices can be multiplied
		if ((*dim)[index1][1] == (*dim)[index2][0]) {
			//save the resulting product matrix
			int **mat_prod = multiply(*mat, *dim, index1, index2);
			//alloc memory for the new matrix and it's dimmensions
			*mat = realloc(*mat, (*nr + 1) * sizeof(int **));
			*dim = realloc(*dim, (*nr + 1) * sizeof(int *));
			(*dim)[*nr] = (int *)malloc(2 * sizeof(int));
			//add new matrix
			(*nr)++;
			(*mat)[*nr - 1] = mat_prod;
			//set new dimensions value
			(*dim)[*nr - 1][0] = (*dim)[index1][0];
			(*dim)[*nr - 1][1] = (*dim)[index2][1];
		} else {
			printf("Cannot perform matrix multiplication\n");
		}
	} else {
		printf("No matrix with the given index\n");
	}
}

void read_mat(int ****mat, int nr, int n, int m)
{
	//read the elements of the new matrix
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			//read element and add it to the matrix
			int element;
			scanf("%d", &element);
			//save the elements
			(*mat)[nr][i][j] = element;
				}
}

void free_m(int **m1, int **m2, int **m3, int **m4,
			int **m5, int **m6, int **m7, int dim)
{
	/* function for freeing the memory allocated for the
	matrices resulting from recursive functions and used
	for the multiplication operations*/
	free_matrix(dim, m1);
	free_matrix(dim, m2);
	free_matrix(dim, m3);
	free_matrix(dim, m4);
	free_matrix(dim, m5);
	free_matrix(dim, m6);
	free_matrix(dim, m7);
}

void free_aux_mat(int ***a, int dim_mat, int dim)
{
	/*function for freeing the memory allocated for array of matrices
	used operations*/
	for (int i = 0; i < dim_mat; i++)
		free_matrix(dim, a[i]);
	free(a);
}

/*function used for performing the operation of addition or
substract, depending on the character that is being read*/
int **mat_op(int **mat1, int **mat2, int n, int decision)
{
	int **op = alloc_mat(n, n);
	if (decision == -1) {
		for (int  i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				op[i][j] = overflow(mat1[i][j] - mat2[i][j]);
	}
	if (decision == 1) {
		for (int  i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				op[i][j] = overflow(mat1[i][j] + mat2[i][j]);
	}
	return op;
}

int **strassen_multiply(int **mat1, int **mat2, int n)
{
	/*if the size of the matrix is equal to 1, then the expected
	result has been reached*/
	if (n == 1) {
		int **str = alloc_mat(1, 1);
		str[0][0] = mat1[0][0] * mat2[0][0];
		return str;
	}
	/*if the size is not equal to 1 yet allocate memory for the matrix
	that will store the result, according to its n sizes */
	int **str = alloc_mat(n, n);
	int dim = n / 2;
	int k = 4;
	//the matrix is splitted into 2 matrices treated as an individual element
	int ***q1 = malloc(4 * sizeof(int **)), ***q2 = malloc(4 * sizeof(int **));
	//each matrix will be splitted in 4 smaller matrices
	for (int i = 0; i < k; ++i) {
		q1[i] = alloc_mat(dim, dim);
		q2[i] = alloc_mat(dim, dim);
	}
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; j++) {
			//initialise matrices using the elements of the first matrix
			q1[0][i][j] = mat1[i][j]; q2[0][i][j] = mat2[i][j];
			q1[1][i][j] = mat1[i][j + dim]; q2[1][i][j] = mat2[i][j + dim];
			q1[2][i][j] = mat1[i + dim][j]; q2[2][i][j] = mat2[i + dim][j];
			q1[3][i][j] = mat1[i + dim][j + dim];
			q2[3][i][j] = mat2[i + dim][j + dim];
		}
	/*first,create an array of matrices that contain
	addition operations are initialised*/
	int ***a = (int ***)malloc(10 * sizeof(int **));
	a[0] = mat_op(q1[0], q1[3], dim, 1);
	a[1] = mat_op(q2[0], q2[3], dim, 1);
	a[2] = mat_op(q1[2], q1[3], dim, 1);
	a[3] = mat_op(q1[0], q1[1], dim, 1);
	a[4] = mat_op(q2[0], q2[1], dim, 1);
	a[5] = mat_op(q2[2], q2[3], dim, 1);
	/*then create an array of matrices and initialised the
	ones that contain substraction operations*/
	int ***s = (int ***)malloc(4 * sizeof(int **));
	s[0] = mat_op(q2[1], q2[3], dim, -1);
	s[1] = mat_op(q2[2], q2[0], dim, -1);
	s[2] = mat_op(q1[2], q1[0], dim, -1);
	s[3] = mat_op(q1[1], q1[3], dim, -1);
	// these recursive matrices will get devided untill they reach size 1 x 1
	int **m1 = strassen_multiply(a[0], a[1], dim);
	int **m2 = strassen_multiply(a[2], q2[0], dim);
	int **m3 = strassen_multiply(q1[0], s[0], dim);
	int **m4 = strassen_multiply(q1[3], s[1], dim);
	int **m5 = strassen_multiply(a[3], q2[3], dim);
	int **m6 = strassen_multiply(s[2], a[4], dim);
	int **m7 = strassen_multiply(s[3], a[5], dim);
	//free memory alocated for matrices used to perform operations
	free_aux_mat(s, 4, dim);
	free_aux_mat(q1, 4, dim);
	free_aux_mat(q2, 4, dim);
	a[6] = mat_op(m1, m4, dim, 1);
	a[7] = mat_op(a[6], m7, dim, 1);
	a[8] = mat_op(m1, m3, dim, 1);
	a[9] = mat_op(a[8], m6, dim, 1);
	//mat_rez contains the matrices that elements of the resulted matrix
	int ***mat_rez = (int ***)malloc(4 * sizeof(int **));
	mat_rez[0] = mat_op(a[7], m5, dim, -1); mat_rez[1] = mat_op(m3, m5, dim, 1);
	mat_rez[2] = mat_op(m2, m4, dim, 1); mat_rez[3] = mat_op(a[9], m2, dim, -1);
	//free memory alocated for matrices used to perform operations
	free_m(m1, m2, m3, m4, m5, m6, m7, dim);
	free_aux_mat(a, 10, dim);
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; ++j) {
			//then the final matrix is built by placing each submatrix in it
			//solve overflow problems
			str[i][j] = overflow(mat_rez[0][i][j]);
			str[i][j + dim] = overflow(mat_rez[1][i][j]);
			str[i + dim][j] = overflow(mat_rez[2][i][j]);
			str[i + dim][j + dim] = overflow(mat_rez[3][i][j]);
		}
	//free matrix used to build the multiplied final matrix
	free_aux_mat(mat_rez, 4, dim);
	return str;
}

void add_strassen(int ****mat, int ***dim, int *nr)
{
	int index1, index2;
	//read the indices of the matrices to be multiplied
	scanf("%d %d", &index1, &index2);

	if (index1 < *nr && index2 < *nr && index1 >= 0 && index2 >= 0) {
		if ((*dim)[index2][0] == (*dim)[index1][1]) {
			//alloc memory for the new matrix and it's dimmensions
			*mat = realloc(*mat, (*nr + 1) * sizeof(int **));
			*dim = realloc(*dim, (*nr + 1) * sizeof(int *));
			(*dim)[*nr] = (int *)malloc(2 * sizeof(int));
			//add new matrix
			int d = (*dim)[index1][0];
			(*mat)[*nr] = strassen_multiply((*mat)[index1], (*mat)[index2], d);
			(*dim)[*nr][0] = (*dim)[index1][0];
			(*dim)[*nr][1] = (*dim)[index1][1];
			(*nr)++;
		} else {
			printf("Cannot perform matrix multiplication\n");
		}
	} else {
		printf("No matrix with the given index\n");
	}
}

int main(void)
{
	//array of matrices
	int ***mat = (int ***)malloc(0 * sizeof(int **));
	//matrix of dimmensions
	int **dim = (int **)malloc(0 * sizeof(int *));
	//nr represents the number of read no.
	int nr = 0;
	char op;
	// read input from keyboard
	scanf("%c", &op);
	// As long as we don't read Q program should run
	while (op != 'Q') {
		//if command starts with L create a new matrix and add it to our array
		if (op == 'L') {
			//initialize the n, m sizes
			int n = 0, m = 0;
			//read the new matrix sizes
			scanf("%d %d", &n, &m);
			//reallocate memory in the array of matrices for the new matrix
			reallocate(&mat, &dim, nr + 1);
			//alloc memory for the matrix
			mat[nr] = alloc_mat(n, m);
			//alloc memory for matrix dimmensions
			dim_alloc_mat(dim, nr);
			read_mat(&mat, nr, n, m);

			//add dimmensions
			dim[nr][0] = n;
			dim[nr][1] = m;
			//increase nr
			nr++;
		}
		if (op == 'F')
			remove_matrix(mat, dim, &nr);

		//if command starts with D read matrix index and print it's dimensions
		if (op == 'D')
			dimensions(dim, nr);

		//if command starts with P read matrix index and print it' s elements
		if (op == 'P')
			display(mat, dim, &nr);

		//if command starts with C use function to resize matrix
		if (op == 'C')
			resize(mat, dim, nr);

		//if command starts with M use function to multiply 2 matrices
		if (op == 'M')
			add_multiplied_mat(&mat, &dim, &nr);

		//if command starts with O sort matrices according to the requierments
		if (op == 'O')
			order(mat, dim, nr);

		//if the command starts with T turn the matrix into its transpose
		if (op == 'T')
			transp(mat, dim, nr);

		//if the command starts with R raise the matrix to power
		if (op == 'R')
			add_power(&mat, dim, nr);

		/*if the command starts with S multiply 2 arrays with the
		Strassen method and add the resulting array to the array*/
		if (op == 'S')
			add_strassen(&mat, &dim, &nr);

		if (!strchr("LDPCOTMFQRS\n", op))
			printf("Unrecognized command\n");
		// read input from keyboard
		scanf("%c", &op);
	}
	//free all the resources
	for (int k = 0; k < nr; k++)
		free_matrix(dim[k][0], mat[k]);
	free(mat);
	free_matrix(nr, dim);
	return 0;
}
