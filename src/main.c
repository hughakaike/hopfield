#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "hopfield.h"
//#include "matrix.h"

double noise_rate=0.1;
double sigma=0.1;
double beta=2.0;

int main(){
    printf("reading config file...\n");
    Image* image=new_image();
    char str[256];

    //config file open
    FILE* fp_config = fopen("config.txt", "r");
    if (fp_config == NULL)
    {
        fprintf(stderr, "Error: config.txt can not open\n");
        exit(1);
    }
    char buffer[256],input_folder[256],output_folder[256],noise_mode[256], hopfield_mode[256];
    while( fscanf(fp_config,"%s",buffer) != EOF ){
        if(strcmp(buffer,"input_folder")==0){
            fscanf(fp_config,"%s",input_folder);
            printf("input folder:%s\n",input_folder);
        }else if(strcmp(buffer,"output_folder")==0){
            fscanf(fp_config,"%s",output_folder);
            printf("output folder:%s\n",output_folder);
        }else if(strcmp(buffer,"noise_mode")==0){
            fscanf(fp_config,"%s",noise_mode);
            printf("noise mode:%s\n",noise_mode);
        }else if(strcmp(buffer,"noise_rate")==0){
            fscanf(fp_config,"%lf",&noise_rate);
            printf("noise rate:%lf\n",noise_rate);
        }else if(strcmp(buffer,"sigma")==0){
            fscanf(fp_config,"%lf",&sigma);
            printf("sigma:%lf\n",sigma);
        }else if(strcmp(buffer,"hopfield_mode")==0){
            fscanf(fp_config,"%s",hopfield_mode);
            printf("hopfield mode:%s\n",hopfield_mode);
        }else if(strcmp(buffer,"beta")==0){
            fscanf(fp_config,"%lf",&beta);
            printf("beta:%lf\n",beta);
        }
    }
    fclose(fp_config);

    //open train_image.dat
    printf("\nreading train images...\n");
    sprintf(str, "%s%s", input_folder,"/matrix/train_images.dat");
    FILE* fp_train_images = fopen(str, "r");
    if (fp_train_images == NULL)
    {
        fprintf(stderr, "Error: train_image.dat can not open\n");
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
        sprintf(str, "%s%s%s", input_folder,"/matrix/", line);
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


    //open test_image.dat
    printf("\nreading test image...\n");
    sprintf(str, "%s%s", input_folder,"/matrix/test_image.dat");
    FILE* fp_test_images = fopen(str, "r");
    if (fp_test_images == NULL)
    {
        fprintf(stderr, "Error: test_image.dat can not open\n");
        exit(1);
    }

    //input test image
    fgets(line, 256, fp_test_images);
    length = strlen(line);
    if (length > 0 && line[length - 1] == '\n') {
        line[--length] = '\0';
    }
    sprintf(str, "%s%s%s", input_folder,"/matrix/", line);
    printf("%s\n", str);
    FILE* fp = fopen(str, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: file can not open\n");
        exit(1);
    }
    read_matrix(fp, &m, &n, &a);
    double** test_image=alloc_matrix(m,n);
    make_test_image(a, test_image, m, n, noise_mode,noise_rate,sigma);
    
    printf("\n");

    //write test image
    sprintf(str, "%s%s", output_folder,"/matrix/test_image.dat");
    FILE* fp_test = fopen(str, "w");
    if (fp_test == NULL)
    {
        fprintf(stderr, "Error: file can not open\n");
        exit(1);
    }
    printf("writing test image...\n");
    fprint_matrix(fp_test, m, n, test_image);
    fclose(fp_test);
    
    Hopfield* hopfield=new_hopfield(image);
    double** output=alloc_matrix(m,n);
    predict_hopfield(hopfield, m,n,test_image,output,hopfield_mode,beta);

    printf("writing result image...\n");
    sprintf(str, "%s%s", output_folder,"/matrix/output_image.dat");
    FILE* fp_output = fopen(str, "w");
    fprint_matrix(fp_output, m, n,output);
    if (fp_output == NULL)
    {
        fprintf(stderr, "Error: file can not open\n");
        exit(1);
    }
    fclose(fp_output);

    return 0;
}