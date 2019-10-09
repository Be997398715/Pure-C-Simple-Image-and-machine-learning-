#include "ALLDEFINE.h"


static float predict_linear_regression(int x, float *w, float *b){
	if(x==0 || w==NULL || b==NULL)	return -1;
	return (float)(*w * x + *b);
}

static float batch_gradient_descent(int *x, int *y, int n, float learning_rate, float *w, float *b){

	float db = 0.0;
	float dw = 0.0;
	float cost = 0.0;

	for(int i=0; i<n; i++){

		cost += pow(((*w * x[i] + *b) - y[i]),2);
		db += (*w * x[i] + *b) - y[i];
		dw += ((*w * x[i] + *b) - y[i])*x[i];
	}

	cost /= n;
	*w = *w - learning_rate*(dw)/n;
	*b = *b - learning_rate*(db)/n;

	return cost;
}


/*
// Function: simple linear regression
// Parameters : X, Y, W1, B
// Learning Rate : 0.01
// Loss Function : Squared (MSE)
*/
u8 image_linear_regression(int *x, int *y, int size, float *cost, float learning_rate, int num_iterations, float *w, float *b)
{
	if(x==NULL || y==NULL || w==NULL || b==NULL || cost==NULL || size<1 || learning_rate==0 || num_iterations==0)	return IMAGE_RET_ERROR;
	
	int n = size;

	for(int i=0; i<num_iterations; i++){
		cost[i] = batch_gradient_descent(x, y, n, learning_rate, w, b);
	}

	return IMAGE_RET_OK;
}


/*
float* linear_regression(int *x, int *y, float learning_rate, int num_iterations, float *w, float *b);
float batch_gradient_descent(int *x, int *y, int n, float learning_rate, float *w, float *b);
float predict(int x, float *w, float *b);

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

	float *cost = linear_regression(x, y, learning_rate, num_iterations, &w, &b);

	printf("Final Value Of w %f \n", w);
	printf("Final Value Of b %f \n \n", b);

	printf("Predicted Value For 10 is %f \n",predict(10,&w,&b));

	printf("Cost Values \n");
	for(int i=0; i<num_iterations; i++){
		printf("Cost %d %f \n",i,cost[i]);
	}


}
*/