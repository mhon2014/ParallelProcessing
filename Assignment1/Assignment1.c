#include <stdio.h>
#include <math.h>
#include <time.h> 

int main(int argc, char *argv[]){
    
    /* Variable declarations */
    clock_t t;
    double time,timesquareroot; // in seconds
    int n;
    double xMid, xDelta, pi= 0, pisqrt = 0,  area = 0;
    
    
    /* User input query */
    printf("Enter number of rectangles: ");
    scanf("%d", &n);                        //Enter number of rectangles


    /* Width and Mid point calculations */
    xDelta = (double)1/n;                   //Calculate the the width of the rectangles
    xMid = xDelta/2;                        //Mid point of the rectangles

    t = clock(); 

    for(double i = 0; i < n; i++){          //loop to sum the area 
        area = (4 / (1 + pow(xMid+(xDelta*i),2))); 
        pi += xDelta*area;
    }

    t = clock() - t;
    time = ((double)t)/CLOCKS_PER_SEC;

    t = clock();
    for(double i = 0; i < n; i++){          //loop to sum the area using square root
        area = 4 *sqrt(1 - pow(xMid+(xDelta*i),2)); 
        pisqrt += xDelta*area;
    } 
    t = clock() - t;
    timesquareroot = ((double)t)/CLOCKS_PER_SEC;

    printf("PI: %0.12f. Time to execute loop: %lf.\n",pi, time);
    printf("PI Square root: %0.12f. Time to execute loop: %lf.\n",pisqrt, timesquareroot);


    return 0;
}