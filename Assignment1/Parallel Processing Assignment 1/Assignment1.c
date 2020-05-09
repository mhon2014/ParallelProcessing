#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]){
    
    /* Variable declarations */
    int n;
    double xMid, xDelta, pi= 0.0,  area = 0;
    
    
    /* User input query */
    printf("Enter number of rectangles: ");
    scanf("%d", &n);                        //Enter number of rectangles


    /* Width and Mid point calculations */
    xDelta = (double)1/n;                   //Calculate the the width of the rectangles
    xMid = xDelta/2;                        //Mid point of the rectangles


    for(double i = 0; i < n; i++){          //loop to sum the area 
        area = (4 / (1 + pow(xMid+(xDelta*i),2))); 
        pi += xDelta*area;
    }

    printf("%lf",pi);

    return 0;