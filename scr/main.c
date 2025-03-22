#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "hopfield.h"
#include "matrix.h"

#define MY_PI 3.141592653589793238462643

const double noise_rate=0.1;
const double sigma=0.1;

double Uniform( void ){
	return ((double)rand()+1.0)/((double)RAND_MAX+2.0);
}

double Normal(double mu,double sigma){
    double X=Uniform();
    double Y=Uniform();
    return sigma*sqrt(-2.0*log(X))*cos(2.0*MY_PI*Y)+mu;
}

double clamp(double x, double min, double max){
    if (x<min){
        return min;
    }else if (x>max){
        return max;
    }else{
        return x;
    }
}

double make_test_image(double** image, double** noisy_image, int m, int n, char* mode){
    if(mode=="flip"){
        for(int i=0; i<m; ++i){
            for(int j=0; j<n; ++j){
                if (Uniform()<noise_rate){
                    noisy_image[i][j]=1.0-image[i][j]/255;
                }else{
                    noisy_image[i][j]=image[i][j]/255;
                }
            }
        }
    }else if(mode=="salt_and_pepper"){
        for(int i=0; i<m; ++i){
            for(int j=0; j<n; ++j){
                if (Uniform()<noise_rate){
                    if (Uniform()<0.5){
                        noisy_image[i][j]=0.0;
                    }else{
                        noisy_image[i][j]=1.0;
                    }
                }else{
                    noisy_image[i][j]=image[i][j]/255;
                }
            }
        }
    }else if(mode=="white"){
        for(int i=0; i<m; ++i){
            for(int j=0; j<n; ++j){
                noisy_image[i][j]=clamp(image[i][j]/255+Uniform(),0.0,1.0);
            }
        }
    }else if(mode=="gaussian"){
        for(int i=0; i<m; ++i){
            for(int j=0; j<n; ++j){
                noisy_image[i][j]=clamp(image[i][j]/255+Normal(0.0,sigma),0.0,1.0);
            }
        }
    }
}

int main(int argc, char *argv[]){
    if(argc!=3){
        fprintf(stderr, "Error: invalid argument\n");
        exit(1);
    }
    Image* image=new_image();

    char str[256];
    sprintf(str, "%s%s", argv[1],"/matrix/train_images.dat");
    FILE* fp_train_images = fopen(str, "r");
    if (fp_train_images == NULL)
    {
        fprintf(stderr, "Error: file can not open\n");
        exit(1);
    }

    sprintf(str, "%s%s", argv[1],"/matrix/test_image.dat");
    FILE* fp_test_images = fopen(str, "r");
    if (fp_test_images == NULL)
    {
        fprintf(stderr, "Error: file can not open\n");
        exit(1);
    }
    
    //input train images
    char line[256];
    size_t length;
    int m, n;
    double** a;
    while (fgets(line, 256, fp_train_images) != NULL) {
        length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') {
            line[--length] = '\0';
        }
        sprintf(str, "%s%s%s", argv[1],"/matrix/", line);
        printf("%s\n", str);
        FILE* fp = fopen(str, "r");
        if (fp == NULL)
        {
            fprintf(stderr, "Error: file can not open\n");
            exit(1);
        }
        read_matrix(fp, &m, &n, &a);
        add_image(image, m, n, a);
    }

    //input test image
    fgets(line, 256, fp_test_images);
    length = strlen(line);
    if (length > 0 && line[length - 1] == '\n') {
        line[--length] = '\0';
    }
    sprintf(str, "%s%s%s", argv[1],"/matrix/", line);
    printf("%s\n", str);
    FILE* fp = fopen(str, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: file can not open\n");
        exit(1);
    }
    read_matrix(fp, &m, &n, &a);
    double** test_image=alloc_matrix(m,n);
    make_test_image(a, test_image, m, n, "gaussian");

    //write test image
    sprintf(str, "%s%s", argv[2],"/matrix/test_image.dat");
    printf("1\n");
    FILE* fp_test = fopen(str, "w");
    if (fp_test == NULL)
    {
        fprintf(stderr, "Error: file can not open\n");
        exit(1);
    }
    //fprint_matrix(stderr, m, n, test_image);
    fprint_matrix(fp_test, m, n, test_image);
    
    Hopfield* hopfield=new_hopfield(image);
    double** output=alloc_matrix(m,n);
    predict_hopfield(hopfield, m,n,test_image,output,"continuous");

    sprintf(str, "%s%s", argv[2],"/matrix/output_image.dat");
    FILE* fp_output = fopen(str, "w");
    fprint_matrix(fp_output, m, n,output);
    if (fp_output == NULL)
    {
        fprintf(stderr, "Error: file can not open\n");
        exit(1);
    }

    return 0;
}