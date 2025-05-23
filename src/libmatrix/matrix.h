#include <stdlib.h>
#include <stdio.h>
#include <complex.h>

#ifndef MATRIX_H
#define MATRIX_H

#define mat_elem(mat, i, j) (mat)[(i)][(j)]

//allocate double vector[m]
double *alloc_vector(int m);

//allocate double matrix[m][n]
double **alloc_matrix(int m, int n);

void read_matrix(FILE *fp, int *m, int *n, double ***mat);

//print double vector[m]
void fprint_vector(FILE *fp, int m, double *vec);

//print double matrix[m][n]
void fprint_matrix(FILE *fp, int m, int n, double **mat);

#endif