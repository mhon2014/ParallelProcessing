#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>



typedef struct matrix{ //struct to store 2d matrix and sizes
    double **values;
    int row,col;
}Matrix;

void initializeMatrix(Matrix *arg_mat, int arg_pagenumber);
void initializeVector(Matrix *arg_mat, int arg_pagenumber);
void matmul(Matrix *arg_gMatrix, Matrix *old, Matrix *new);
void printstuff(Matrix *arg_gMatrix);
void normalize(Matrix *arg_vector);
void findminmax(Matrix *arg_vector);

//initialize Gmatrix
void initializeMatrix(Matrix *arg_mat, int arg_pagenumber){ //initialize the google matrix
    double dfactor = 0.15;

    arg_mat->row = arg_pagenumber;
    arg_mat->col = arg_pagenumber;
    arg_mat->values = (double**)malloc(arg_pagenumber * sizeof(double*));
    for (int i = 0; i < arg_pagenumber; i++){ //make space in the heap for 2d matrix
        arg_mat->values[i] = (double*)malloc(arg_pagenumber * sizeof(double));
    }

    for (int i = 0; i < arg_mat->row; i++){
        for (int j = 0; j < arg_mat->col; j++){
            arg_mat->values[i][j] = 0;
        }
    }

    arg_mat->values[0][arg_pagenumber-1] = 0.5; //first row last collumn initialize
    arg_mat->values[1][0] = 1;              //second row first column initialize

    for (int row = 0, column = 1; row < arg_pagenumber - 1; row++, column++){ //first row first column
        arg_mat->values[row][column] = 0.5;
    }

    for (int row = 2, column = 1; row < arg_pagenumber; row++, column++){ //second row first column 
        arg_mat->values[row][column] = 0.5;
    }

    // printstuff(arg_mat);

    //google matrix initialize
    for(int i = 0; i < arg_pagenumber; i++){
        for(int j = 0; j < arg_pagenumber; j++){
            arg_mat->values[i][j] = ( (1.0-dfactor) * (arg_mat->values[i][j]) ) + ( dfactor * (1.0/arg_pagenumber) );
        }
    }
}

//initiliaze pagerank vector
void initializeVector(Matrix *arg_mat, int arg_pagenumber){
    arg_mat->row = arg_pagenumber;
    arg_mat->col = 1;
    arg_mat->values = (double**)malloc(arg_pagenumber * sizeof(double*));
    for (int i = 0; i < arg_pagenumber; i++){ //make space in the heap for 2d matrix
        arg_mat->values[i] = (double*)malloc(arg_pagenumber * sizeof(double));
        arg_mat->values[i][0] = 1.0 / arg_pagenumber;
    }
}

//matvec stuff
void matmul(Matrix *arg_gMatrix, Matrix *old, Matrix *new){
    //actual dot product / matrix multiplication? y = Gx
    // printstuff(arg_gMatrix);
    // double tmp;
    #pragma omp parallel for
    for(int i = 0; i < arg_gMatrix->row; i++){
        for(int j = 0; j < arg_gMatrix->col; j++){
            new->values[i][0] += arg_gMatrix->values[i][j]*old->values[j][0];
            // printf("%lf x %lf \n", arg_gMatrix->values[i][j], old->values[j][0]);
            // printf("\n");
        }
        // printf("%lf", tmp);
        // printf("\n\n");
        // new->values[i][0] = tmp;
    }

    normalize(new);
    
    for(int i = 0; i < arg_gMatrix->row; i++){
        old->values[i][0] = new->values[i][0]; // x = y
    }

}

//normalize values, they weren't adding up to one
void normalize(Matrix *arg_vector){
    double sum = 0;
    for(int i = 0; i < arg_vector->row; i++){
        sum += arg_vector->values[i][0];
    }

    for(int i = 0; i < arg_vector->row; i++){
        arg_vector->values[i][0] /= sum;
    }
}

void findminmax(Matrix *arg_vector){ //find min max in pagerank assuming no zeros
    int mini = 0, maxi = 0;
    double minvalue = arg_vector->values[0][0], maxvalue = minvalue;

    for(int i = 0; i < arg_vector->row; i++){
        if (minvalue >= arg_vector->values[i][0]){
            minvalue =  arg_vector->values[i][0];
            mini = i;
        }

        if(maxvalue <= arg_vector->values[i][0]){
            maxvalue = arg_vector->values[i][0];
            maxi = i;
        }
    }
    
    printf("Min: Pagerank[%d] = %lf \n", mini, minvalue);
    printf("Max: Pagerank[%d] = %lf \n", maxi, maxvalue);
}

//print matrix
void printstuff(Matrix *arg_gMatrix){
      for(int i = 0; i < arg_gMatrix->row; i++){
        for(int j = 0; j < arg_gMatrix->col; j++){
            printf("%lf ", arg_gMatrix->values[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    int pagenumber = 16;

    double startTime, endTime;


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
    
    startTime = omp_get_wtime();

    for (int i = 0; i < 1000; i++){
        matmul(gMatrix, pagerank, result);
    }

    endTime = omp_get_wtime();

    printstuff(result);

    // double sum = 0;
    // for (int i = 0; i < result->row; i++){
    //     sum += result->values[i][0];
    // }
    // printf("Sum: %lf\n", sum);

    findminmax(result);

    printf("Runtime: %.16e\n", endTime - startTime);

    return 0;
}
