/*
useful functions for matrix operations
*/

#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include "matrix.h"

#define mat_elem(mat, i, j) (mat)[(i)][(j)]

//allocate double vector[m]
inline double *alloc_vector(int m) {
  double *vec;
  vec = (double*)malloc((size_t)(m * sizeof(double)));
  for(int i=0;i<m;++i){
      vec[i]=0;
  }
  return vec;
}

//allocate double matrix[m][n]
inline double **alloc_matrix(int m, int n) {
  int i;
  double **mat;
  mat = (double**)malloc((size_t)(m * sizeof(double*)));
  for(i=0;i<m;i++){
      mat[i] = (double *)calloc(n,sizeof(double));
  }
  return mat;
}

inline void read_matrix(FILE *fp, int *m, int *n, double ***mat) {
  fscanf(fp, "%d", m);
  fscanf(fp, "%d", n);
  *mat = alloc_matrix(*m,*n);
  for (int i=0; i<*m; ++i) {
    for (int j=0;j<*n; ++j){
      fscanf(fp, "%lf", &mat_elem(*mat, i, j));
    }
  }
}

//print double vector[m]
inline void fprint_vector(FILE *fp, int m, double *vec) {
  int i;
  fprintf(fp, "%d\n", m);
  for (i = 0; i < m; ++i) {
    fprintf(fp, "%10.3f ", vec[i]);
  }
}

//print double matrix[m][n]
inline void fprint_matrix(FILE *fp, int m, int n, double **mat) {
  int i, j;
  fprintf(fp, "%d %d\n", m, n);
  for (i = 0; i < m; ++i) {
    for (j = 0; j < n; ++j) fprintf(fp, "%10.3f ", mat[i][j]);
    fprintf(fp, "\n");
  }
}