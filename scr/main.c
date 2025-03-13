#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "hopfield.h"
#include "matrix.h"

const double noise_rate=0.1;

double Uniform( void ){
	return ((double)rand()+1.0)/((double)RAND_MAX+2.0);
}

double test_data[9][9]=
    {{1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,0,0,1,1},
    {1,1,1,1,1,0,0,1,1},
    {1,1,1,1,0,1,1,1,1},
    {1,1,0,1,0,0,1,1,1},
    {1,1,0,0,0,0,1,1,1},
    {1,1,1,1,1,0,1,1,1},
    {1,1,1,1,1,1,1,1,1},
    };

int main(){
    Image* image=new_image();
    char input_dir[256]="hopfield/input";

    char str[256];
    sprintf(str, "%s%s", input_dir,"/matrix/train_images.dat");
    FILE* fp_train_images = fopen(str, "r");
    if (fp_train_images == NULL)
    {
        fprintf(stderr, "Error: file can not open\n");
        exit(1);
    }

    sprintf(str, "%s%s", input_dir,"/matrix/test_image.dat");
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
        sprintf(str, "%s%s%s", input_dir,"/matrix/", line);
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
    sprintf(str, "%s%s%s", input_dir,"/matrix/", line);
    printf("%s\n", str);
    FILE* fp = fopen(str, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: file can not open\n");
        exit(1);
    }
    read_matrix(fp, &m, &n, &a);
    double** test_image=alloc_matrix(m,n);
    for(int i=0; i<m; ++i){
        for(int j=0; j<n; ++j){
            if (Uniform()<noise_rate){
                test_image[i][j]=(255-a[i][j])/255;
            }else{
                test_image[i][j]=a[i][j]/255;
            }
        }
    }

    for(int i=0; i<m; ++i){
        for(int j=0; j<n; ++j){
            printf("%1.0lf ", test_image[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    Hopfield* hopfield=new_hopfield(image);
    train_hopfield(hopfield,"hebbian");
    double** output=alloc_matrix(m,n);
    predict_hopfield(hopfield, m,n,test_image,output,"continuous");
    for(int i=0; i<m; ++i){
        for(int j=0; j<n; ++j){
            printf("%1.0lf ", output[i][j]);
        }
        printf("\n");
    }

    FILE* fpa = fopen("hopfield/test_log.dat", "w");
    //fprint_matrix(fpa, m*n, m*n, hopfield->weight);
    fprint_matrix(fpa, 2, m*n, image->image);
    
    return 0;
}