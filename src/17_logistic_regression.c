#include "ALLDEFINE.h"

static double sigmoid(double x)
{
	double ex;
	ex = pow(2.718281828,x);
	return ex/(1+ex);
}
 
 
/*
\\\\ Function name: logistic regression
\\\\ Parameters: 
	 1> x : two-dimensions data for train
	 2> y : labels of train data
	 3> x_size : size of one of train_data
	 4> y_size : size of labels (`0` or `1`)
	 5> learning_rate : learning_rate, adjust for gradient descent
	 6> num_iterations : train iterations
	 7> theta : weight of each data
*/ 
u8 image_logistic_regression(double ** x, int * y, double* theta, int x_size, int y_size, float learning_rate, int num_iterations)
{	
	if (x_size < 1 || x == NULL || y == NULL || y_size < 1 || theta == NULL || learning_rate == 0 || num_iterations == 0)	return IMAGE_RET_ERROR;

    int i,j,k;
    double h,error,derivate;
	
    for(i=0; i<num_iterations; i++)
    {
        for(j=0;j<y_size;j++)
        {
            h=0;
            for(k=0;k<x_size;k++)
            {
                h += x[j][k]*theta[k];
            }
            
			// backward propagation
			error = sigmoid(h) - y[j];
			
            for(k=0;k<x_size;k++)
            {
                derivate = (-1)*error*x[j][k];
                theta[k] += learning_rate*derivate;
            }
        }
    }
 
	return IMAGE_RET_OK;
}

u8 image_predict_logistic_regression(int** x, int* result, int x_size, int y_size, double* theta, double threshold)
{
	if (x_size < 1 || x == NULL || result == NULL || y_size < 1 || theta == NULL)	return IMAGE_RET_ERROR;

	int i, j;
	double h;
	
    for(i=0; i<x_size; i++)
    {
		h = 0;
	    for(j=0; j<y_size; j++)
		{
			 h += x[i][j]*theta[j];
		}
		
		result[i] = h > threshold ? 1 : 0;
	}
	
	return IMAGE_RET_OK;
}


/*
int main(int argc, char *argv[]){


	//Input Data
	int x[10] = {1,2,3,4,5,6,7,8,9,10};
	int y[10] = {2,4,6,9,15,12,15,17,20,30};

	//Parameters
	float learning_rate = 0.01;
	int num_iterations = 100;

	//Initialize Parameters
	srand(time(NULL));
	float w = rand() % (1 - 0 + 1) + 0;
	float b = 0;

	printf("Inital Value Of w %f \n", w);
	printf("Initial Value Of b %f \n", b);

	float *cost = logistic_regression(x, y, learning_rate, num_iterations, &w, &b);

	printf("Final Value Of w %f \n", w);
	printf("Final Value Of b %f \n \n", b);

	printf("Predicted Value For 10 is %f \n",predict_logistic_regression(10,&w,&b));

	printf("Cost Values \n");
	for(int i=0; i<num_iterations; i++){
		printf("Cost %d %f \n",i,cost[i]);
	}


	exit(0);

}
*/