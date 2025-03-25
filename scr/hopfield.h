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

double inner_product(double* vec1, double* vec2, int n){
    double result=0;
    for(int i=0;i<n;++i){
        result+=vec1[i]*vec2[i];
    }
    return result;
}

double inner_product2(double* vec1, double* vec2, int n){
    double result=0;
    for(int i=0;i<n;++i){
        result+=(2*vec1[i]-1)*(2*vec2[i]-1);
    }
    return result;
}

//m:n_data n:dim
double lse(double b, double** mat, double* vec, int m, int n){
    double result=0;
    for(int i=0;i<m;++i){
        result+=exp(b*inner_product2(mat[i],vec,m));
    }
    return log(result)/b;
}

double lse_flipped(double b, double** mat, double* vec, int m, int n, int flip){
    double result=0;
    for(int i=0;i<m;++i){
        result+=exp(b*inner_product2(mat[i],vec,m)-2*(2*vec[flip]-1)*(2*mat[i][flip]-1));
    }
    return log(result)/b;
}

void softmax(double b, double** mat, double* vec, double* result, int m, int n){
    double Z=0.0;
    double max=b*inner_product2(mat[0],vec,n);
    for(int i=1;i<m;++i){
        if(b*inner_product2(mat[i],vec,n)>max){
            max=b*inner_product2(mat[i],vec,n);
        }
    }
    for(int i=0;i<m;++i){
        Z+=exp(b*inner_product2(mat[i],vec,n)-max);
    }
    for(int i=0;i<m;++i){
        result[i]=exp(b*inner_product2(mat[i],vec,n)-max)/Z;
    }
}

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

double make_test_image(double** image, double** noisy_image, int m, int n, char* mode, double noise_rate, double sigma){
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


Hopfield* init_hopfield(int XN, int YN, Image* images){
    Hopfield* hopfield=(Hopfield*)malloc(sizeof(Hopfield));
    hopfield->XN=XN;
    hopfield->YN=YN;
    hopfield->threshold=(double*)malloc(XN*YN*sizeof(double));
    hopfield->images=images;
    for(int i=0;i<XN;++i){
        for(int j=0;j<YN;++j){
            hopfield->threshold[i+j*XN]=0.0;
        }
    }
    return hopfield;
}

Hopfield* new_hopfield(Image* images){
    Hopfield* hopfield=(Hopfield*)malloc(sizeof(Hopfield));
    int XN=images->XN;
    int YN=images->YN;
    return init_hopfield(XN, YN, images);
}

void predict_hopfield_calssic(Hopfield* hopfield, int input_XN,int input_YN,double** input,double** output){
    double* input_vec=alloc_vector(input_XN*input_YN);
    for(int i=0;i<input_XN;++i){
        for(int j=0;j<input_YN;++j){
            input_vec[i+j*input_XN]=input[i][j];
        }
    }
    int XN=hopfield->XN;
    int YN=hopfield->YN;
    Image* images=hopfield->images;
    int n_data=images->n_data;
    double* predict=alloc_vector(XN*YN);
    int change_flag=1;
    double weight;
    while (change_flag==1){
        change_flag=0;
        for(int i=0;i<XN*YN;++i){
            predict[i]=0.0;
            for(int j=0;j<XN*YN;++j){
                weight=0.0;
                for(int k=0;k<n_data;++k){
                    weight+=(2*images->image[k][i]-1)*(2*images->image[k][j]-1)/n_data;
                }
                predict[i]+=weight*(2*input_vec[j]-1);
            }
            if (predict[i]>hopfield->threshold[i]){
                predict[i]=1.0;
            }else{
                predict[i]=0.0;
            }
            if(predict[i]!=input_vec[i]){
                change_flag=1;
            }
        }
        for(int i=0;i<XN*YN;++i){
            input_vec[i]=predict[i];
        }
    }
    for(int i=0;i<input_XN;++i){
        for(int j=0;j<input_YN;++j){
            output[i][j]=input_vec[i+j*input_XN];
        }
    }
    free(input_vec);
    free(predict);
}

void predict_hopfield_modern(Hopfield* hopfield, int input_XN,int input_YN,double** input,double** output){
    double* input_vec=alloc_vector(input_XN*input_YN);
    for(int i=0;i<input_XN;++i){
        for(int j=0;j<input_YN;++j){
            input_vec[i+j*input_XN]=input[i][j];
        }
    }
    int XN=hopfield->XN;
    int YN=hopfield->YN;
    Image* images=hopfield->images;
    int n_data=images->n_data;
    int change_flag=1;
    double energy_diff;
    while (change_flag==1){
        change_flag=0;
        for(int i=0;i<XN*YN;++i){ 
            energy_diff=0;
            for (int n=0;n<n_data;++n){
                energy_diff+=exp(inner_product2(images->image[n],input_vec,XN*YN)-2*(2*input_vec[i]-1)*(2*images->image[n][i]-1))-exp(inner_product2(images->image[n],input_vec,XN*YN));
            }
            printf("%lf\n",energy_diff);
            if (energy_diff>0){
                if (input_vec[i]==0.0){
                    input_vec[i]=1.0;
                }else{
                    input_vec[i]=0.0;
                }
                change_flag=1;
            }
        }
    }
    for(int i=0;i<input_XN;++i){
        for(int j=0;j<input_YN;++j){
            output[i][j]=input_vec[i+j*input_XN];
        }
    }
    free(input_vec);
}

void predict_hopfield_coutinuous(Hopfield* hopfield, int input_XN,int input_YN,double** input,double** output){
    double* input_vec=alloc_vector(input_XN*input_YN);
    for(int i=0;i<input_XN;++i){
        for(int j=0;j<input_YN;++j){
            input_vec[i+j*input_XN]=input[i][j];
        }
    }
    int XN=hopfield->XN;
    int YN=hopfield->YN;
    Image* images=hopfield->images;
    int n_data=images->n_data;
    double* predict=alloc_vector(XN*YN);
    double* softmax_result=alloc_vector(n_data);
    softmax(4.0,images->image,input_vec,softmax_result,n_data,XN*YN);
    for(int i=0;i<XN*YN;++i){
        for(int j=0;j<n_data;++j){
            predict[i]+=softmax_result[j]*images->image[j][i];
        }
        input_vec[i]=predict[i];
    }
    for(int i=0;i<input_XN;++i){
        for(int j=0;j<input_YN;++j){
            output[i][j]=input_vec[i+j*input_XN];
        }
    }
    free(input_vec);
    free(predict);
    free(softmax_result);
}

void predict_hopfield(Hopfield* hopfield, int input_XN,int input_YN,double** input,double** output, char* mode){
    int XN=hopfield->XN;
    int YN=hopfield->YN;
    if (XN!=input_XN || YN!=input_YN){
        printf("The size of the input is different.\n");
    }else{
        if (mode=="classic"){
            predict_hopfield_calssic(hopfield,input_XN,input_YN,input,output);
        }else if(mode=="modern"){
            predict_hopfield_modern(hopfield,input_XN,input_YN,input,output);
        }else if(mode=="continuous"){
            predict_hopfield_coutinuous(hopfield,input_XN,input_YN,input,output);
        }
    }
}

Image* init_image(){
    Image* image=(Image*)malloc(sizeof(Image));
    image->XN=0;
    image->YN=0;
    image->n_data=0;
    return image;
}

Image* new_image(){
    Image* image=(Image*)malloc(sizeof(Image));
    return init_image();
}

void add_image(Image* image, int XN, int YN, double** data){
    int n_data=image->n_data;
    if (n_data==0){
        image->XN=XN;
        image->YN=YN;
        image->n_data=1;
        image->image=(double**)malloc(sizeof(double*));
        image->image[0]=(double*)malloc(XN*YN*sizeof(double));
        for(int i=0;i<XN;++i){
            for(int j=0;j<YN;++j){
                image->image[0][i+j*XN]=data[i][j]/255;
            }
        }
    }else{
        if (XN==image->XN && YN==image->YN){
            image->n_data++;
            int n_data=image->n_data;
            double** prev_image=image->image;
            double** image_matrix=alloc_matrix(n_data, XN*YN);
            image->image=image_matrix;
            for(int i=0;i<n_data-1;++i){
                for(int j=0;j<XN*YN;++j){
                    image->image[i][j]=prev_image[i][j];
                }
            }
            for(int i=0;i<XN;++i){
                for(int j=0;j<YN;++j){
                    image->image[n_data-1][i+j*XN]=data[i][j]/255;
                }
            }
        }else{
            printf("The size of the image is different from the previous one.\n");
        }
    }
}