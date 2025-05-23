#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

#define MY_PI 3.141592653589793238462643

/*
################
#math functions#
################
*/

double inner_product(double* vec1, double* vec2, int n);

double inner_product2(double* vec1, double* vec2, int n);

//m:n_data n:dim
double lse(double b, double** mat, double* vec, int m, int n);

double lse_flipped(double b, double** mat, double* vec, int m, int n, int flip);

void softmax(double b, double** mat, double* vec, double* result, int m, int n);

double Uniform( void );

double Normal(double mu,double sigma);

double clamp(double x, double min, double max);

void make_test_image(double** image, double** noisy_image, int m, int n, char* mode, double noise_rate, double sigma);
/*
################
#data structure#
################
*/


typedef struct image_data{
    int XN;
    int YN;
    int n_data;
    double** image;
} Image;

typedef struct hopfield_data{
    int XN;
    int YN;
    double* threshold;
    Image* images;
} Hopfield;



Hopfield* init_hopfield(int XN, int YN, Image* images);

Hopfield* new_hopfield(Image* images);

void predict_hopfield_calssic(Hopfield* hopfield, int input_XN,int input_YN,double** input,double** output);

void predict_hopfield_modern(Hopfield* hopfield, int input_XN,int input_YN,double** input,double** output);

void predict_hopfield_coutinuous(Hopfield* hopfield, int input_XN,int input_YN,double** input,double** output, double beta);

void predict_hopfield(Hopfield* hopfield, int input_XN,int input_YN,double** input,double** output, char* mode, double beta);

Image* init_image();

Image* new_image();

void add_image(Image* image, int XN, int YN, double** data);
