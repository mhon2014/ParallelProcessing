#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define dfactor 0.15

typedef struct vec{
    double *values;
    int row;
}Vector;

typedef struct matrix{
    int pagenumber;
    int *iA, *jA; 
    double *A;
    int iAsize, jAsize, Asize;
}SMatrix;

void initializeMatrix(SMatrix *arg_mat, int arg_pagenumber);
void initializeVector(Vector *arg_mat, int arg_pagenumber);
void initializeZero(Vector *arg_mat, int arg_pagenumber);
void matmul(SMatrix *arg_gMatrix, Vector *old, Vector *new);
void normalize(Vector *arg_vector);
void findminmax(Vector *arg_vector);
void printmatrix(SMatrix *arg_Matrix);
void printstuff(Vector *arg_gMatrix);
void destroy(SMatrix *arg_gMatrix);

//initialize sparse matrix
void initializeMatrix(SMatrix *arg_mat, int arg_pagenumber){
    arg_mat->pagenumber = arg_pagenumber;
    arg_mat->Asize = (arg_pagenumber*2) - 1; //n*2-1
    arg_mat->iAsize = (arg_pagenumber + 1); //n+1
    arg_mat->jAsize = arg_mat->Asize;

    arg_mat->A = (double*)malloc(arg_mat->Asize * sizeof(double));
    arg_mat->iA = (int*)malloc(arg_mat->iAsize * sizeof(int));
    arg_mat->jA = (int*)malloc(arg_mat->jAsize * sizeof(int));
    //initialize values?

    //iA pattern of pagenumber *2 untill n-1

    for(int i = 0; i < arg_pagenumber; i++){
        arg_mat->iA[i] = i*2;
    }

    arg_mat->iA[arg_pagenumber] = arg_mat->Asize; //last index of iA


    //initialize first 2 adn last index

    arg_mat->jA[0] = 1;
    arg_mat->jA[1] = arg_pagenumber - 1;
    arg_mat->jA[arg_pagenumber] = arg_pagenumber - 2;

    for(int i = 2, j = 0; j < (arg_mat->jAsize)-3; i += 2, j++){
        arg_mat->jA[i] = j;
        arg_mat->jA[i+1] = j + 2;
    }
    

    //initialize first 3 and then the rest
    arg_mat->A[0] = 0.5;
    arg_mat->A[1] = 0.5;
    arg_mat->A[2] = 1.0;

    for(int i = 3; i < arg_mat->Asize; i++){
        arg_mat->A[i] = 0.5;
    }

}

//set pagerank vector
void initializeVector(Vector *arg_mat, int arg_pagenumber){
    arg_mat->row = arg_pagenumber;
    arg_mat->values = (double*)malloc(arg_pagenumber * sizeof(double));
    for (int i = 0; i < arg_pagenumber; i++){ //make space in the heap for 2d matrix
        arg_mat->values[i] = 1.0 / arg_pagenumber;
    }
}

//set vector to zero
void initializeZero(Vector *arg_mat, int arg_pagenumber){
    arg_mat->row = arg_pagenumber;
    arg_mat->values = (double*)malloc(arg_pagenumber * sizeof(double));
    for (int i = 0; i < arg_pagenumber; i++){ //make space in the heap for 2d matrix
        arg_mat->values[i] = 0;
    }
}

//matvec stuff
void matmul(SMatrix *arg_gMatrix, Vector *old, Vector *new){   
    
    #pragma omp parallel for
    for (int i = 0; i < arg_gMatrix->pagenumber; i++){
        for (int j = arg_gMatrix->iA[i]; j < arg_gMatrix->iA[i+1]; ++j){
            new->values[i] += (1 - dfactor) * arg_gMatrix->A[j] * old->values[arg_gMatrix->jA[j]];
            // printf("%lf * %lf \n", arg_gMatrix->A[j], old->values[arg_gMatrix->jA[j]]);
        }
        new->values[i] += (dfactor / arg_gMatrix->pagenumber);
        // printf("\n");
    }

   normalize(new);

    for(int i = 0; i < arg_gMatrix->pagenumber; i++){
        old->values[i] = new->values[i]; // x = y
    }
}

void findminmax(Vector *arg_vector){ //find min max in pagerank assuming no zeros
    int mini = 0, maxi = 0;
    double minvalue = arg_vector->values[0], maxvalue = minvalue;

    for(int i = 0; i < arg_vector->row; i++){
        if (minvalue >= arg_vector->values[i]){
            minvalue =  arg_vector->values[i];
            mini = i;
        }

        if(maxvalue <= arg_vector->values[i]){
            maxvalue = arg_vector->values[i];
            maxi = i;
        }
    }
    
    printf("Min: Pagerank[%d] = %lf \n", mini, minvalue);
    printf("Max: Pagerank[%d] = %lf \n", maxi, maxvalue);
}
//normalize, my values weren't adding up to one
void normalize(Vector *arg_vector){
    double sum = 0;
    for(int i = 0; i < arg_vector->row; i++){
        sum += arg_vector->values[i];
    }

    for(int i = 0; i < arg_vector->row; i++){
        arg_vector->values[i] /= sum;
    }
}
//print matrix
void printmatrix(SMatrix *arg_Matrix){
    for(int i = 0; i < arg_Matrix->iAsize; i++){
        printf("%d ", arg_Matrix->iA[i]);        
    }

    printf("\n");

    for(int i = 0; i < arg_Matrix->jAsize; i++){
        printf("%d ", arg_Matrix->jA[i]);  
    }

    printf("\n");

    for(int i = 0; i < arg_Matrix->Asize; i++){
        printf("%lf ", arg_Matrix->A[i]);  
    }

    printf("\n");
}

//print vector
void printstuff(Vector *arg_gMatrix){
      for(int i = 0; i < arg_gMatrix->row; i++){
        printf("%lf ", arg_gMatrix->values[i]);
        printf("\n");
    }
}

void destroy(SMatrix *arg_gMatrix){
    free(arg_gMatrix->iA);
    free(arg_gMatrix->jA);
    free(arg_gMatrix->A);
}

int main(int argc, char* argv[]) {

    int pagenumber = 16;

    double startTime, endTime;

    if(argc > 1){
        pagenumber = atoi(argv[1]); //set if argument is present
    }
    
    SMatrix *gMatrix = (SMatrix*)malloc(sizeof(SMatrix));
    initializeMatrix(gMatrix, pagenumber);

    // printmatrix(gMatrix);

    Vector *pagerank = (Vector*)malloc(sizeof(Vector));
    initializeVector(pagerank, pagenumber);

    // printstuff(pagerank);

    //function(matrix, old, new?)
    //dot product / matrix multiplication

    // temporary matrix???
    Vector *result = (Vector*)malloc(sizeof(Vector));
    initializeZero(result, pagenumber);

    startTime = omp_get_wtime();

    // #pragma omp parallel for
    for (int k = 0; k < 1000; k++){
        matmul(gMatrix, pagerank, result);
    
    }

    endTime = omp_get_wtime();

    printstuff(result);

    findminmax(result);

    printf("Runtime: %.16e\n", endTime - startTime);

    destroy(gMatrix);

    free(gMatrix);

    return 0;
}