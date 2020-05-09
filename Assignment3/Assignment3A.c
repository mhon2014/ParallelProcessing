#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <omp.h>



typedef struct matrix{
    double **data;
    int row,col;
}Matrix;

void initializeMatrix(Matrix *arg_mat, int arg_pagenumber){
    double dfactor = 0.15;

    arg_mat->row = arg_pagenumber;
    arg_mat->col = arg_pagenumber;
    arg_mat->data = (double**)malloc(arg_pagenumber * sizeof(double*));
    for (int i = 0; i < arg_pagenumber; i++){ //make space in the heap for 2d matrix
        arg_mat->data[i] = (double*)malloc(arg_pagenumber * sizeof(double));
    }

    arg_mat->data[0][arg_pagenumber-1] = 0.5; //first row last collumn initialize
    arg_mat->data[1][0] = 1;              //second row first column initialize

    for (int row = 0, column = 1; row < arg_pagenumber - 1; row++, column++){ //first row first column
        arg_mat->data[row][column] = .5;
    }

    for (int row = 2, column = 1; row < arg_pagenumber; row++, column++){ //second row first column 
        arg_mat->data[row][column] = .5;
    }

    //google matrix initialize
    for(int i = 0; i < arg_pagenumber; i++){
        for(int j = 0; j < arg_pagenumber; j++){
            arg_mat->data[i][j] = ( (1.0-dfactor) * (arg_mat->data[i][j]) ) + ( dfactor * (1.0/arg_pagenumber) );
        }
    }
}

void initializeVector(Matrix *arg_mat, int arg_pagenumber){
    arg_mat->row = arg_pagenumber;
    arg_mat->col = 1;
    arg_mat->data = (double**)malloc(arg_pagenumber * sizeof(double*));
    for (int i = 0; i < arg_pagenumber; i++){ //make space in the heap for 2d matrix
        arg_mat->data[i] = (double*)malloc(arg_pagenumber * sizeof(double));
        arg_mat->data[i][0] = 1.0 / arg_pagenumber;
    }
}

void matmul(Matrix *arg_gMatrix, Matrix *old, Matrix *new){
    //actual dot product / matrix multiplication? y = Gx
    double tmp;
    for(int i = 0; i < arg_gMatrix->row; i++){
        tmp = 0;
        for(int j = 0; j < arg_gMatrix->col; j++){
            tmp += arg_gMatrix->data[i][j]*old->data[j][0];
        }
        new->data[i][0] = tmp;
    }

    old->data = new->data; // x = y
}

void printstuff(Matrix *arg_gMatrix){
      for(int i = 0; i < arg_gMatrix->row; i++){
        for(int j = 0; j < arg_gMatrix->col; j++){
            printf("%f ", arg_gMatrix->data[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    int pagenumber = 16;

    if(argc > 1){
        pagenumber = atoi(argv[1]); //set if argument is present
    }
    
    Matrix *gMatrix = (Matrix*)malloc(sizeof(Matrix));
    initializeMatrix(gMatrix, pagenumber);
    //vector


    Matrix *pagerank = (Matrix*)malloc(sizeof(Matrix));
    initializeVector(pagerank, pagenumber);


    //function(matrix, old, new?)
    //dot product / matrix multiplication

    //temporary matrix???
    Matrix *result = (Matrix*)malloc(sizeof(Matrix));
    initializeVector(result, pagenumber);

    for (int i = 0; i < 1000; i++){
        matmul(gMatrix, pagerank, result);
    }

    printstuff(result);


    return 0;
}
