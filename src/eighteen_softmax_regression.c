#include "ALLDEFINE.h"

static double fun_ex(x){
	return pow(2.718,x) ;
}

/*
\\\\ Function name: softmax_regression
\\\\ Parameters: 
	 1> x : two-dimensions data for train
	 2> y : labels of train data
	 3> x_size : size of one of train_data
	 4> y_size : size of labels(every number:  `1` `2` etc. and `0` for none class label)
	 5> learning_rate : learning_rate, adjust for gradient descent
	 6> num_iterations : train iterations
	 7> theta : weight of each class of data
	 8> verbose : whether u like to show training information
*/ 
u8 image_softmax_regression(double** x, int* y, double** theta, int x_size, int y_size, int classes, float learning_rate, int num_iterations, int verbose)
{	
	if (x_size < 1 || x == NULL || y == NULL || y_size < 1 || theta == NULL || learning_rate == 0 || num_iterations == 0 || classes < 2)	return IMAGE_RET_ERROR;

	double sum=0,h=0,temp_sum=0;
	double* h_ = (double*)malloc(sizeof(double)*(classes-1));
	double* p_ = (double*)malloc(sizeof(double)*classes);
	int i,j,k,l,n;
	
	// Iteration training
	for(i=0; i<num_iterations; i++)
	{
		for(j=0; j<x_size; j++)
		{
			//equation like NG
			sum = 1;	
			for(l=0; l<(classes-1); l++)
			{
				h = 0;
				for(k=0; k<y_size; k++)
				{
					h = h + theta[l][k]*x[j][k];
				}
				sum = sum + fun_ex(h);
			}
		  
		  
			//Calculate probability of each class
			for(n=0; n<(classes-1); n++)
			{
				h_[n] = 0;
				for(k=0;k<y_size;k++)
				{
					h_[n] = h_[n] + theta[n][k]*x[j][k];
				}
				p_[n] = fun_ex(h_[n]) / sum;
				temp_sum = temp_sum + p_[n];
			}
			p_[classes] = 1 - temp_sum;

			//backward gradient descent
			for(k=0;k<y_size;k++){
				for(n=0; n<(classes-1); n++)
				{
					if(y[j]==(n+1))
					{
						theta[n][k]=theta[n][k]+learning_rate*(1-p_[n])*x[j][k];
					}
					else
					{
						theta[n][k]=theta[n][k]+learning_rate*(-p_[n])*x[j][k];
					}
				}
			}
		}
		
		//output theta and probability information
		if(verbose)
		{
			printf("this is %d iter:\r\n", i);
			for(k=0;k<y_size;k++)
			{
				for(l=0;l<(classes-1);l++) 
				{
					printf("theta[%d][%d]=%1f\n",l,k,theta[l][k]);
					printf("p%d=%1f\n", l+1, p_[l]);
				}
				printf("p%d=%1f\n", classes, p_[classes]);
			}
		}
	}

	// free malloc values
	free(h_);
	free(p_);
	
	return IMAGE_RET_OK;
}


u8 image_predict_softmax_regression(int ** x, int* result, int x_size, int y_size, double** theta, int classes, double threshold)
{
	if(x_size = 0 || x==NULL || result==NULL || y_size==0 || theta==NULL || classes<2)	return IMAGE_RET_ERROR;
	
	double sum=0,h=0,temp_sum=0,temp=0;
	double* h_ = (double*)malloc(sizeof(double)*(classes-1));
	double* p_ = (double*)malloc(sizeof(double)*classes);
	int j,k,l,n,class;
	
	for(j=0; j<x_size; j++)
	{
		sum = 1;		//equation like NG
		for(l=0; l<(classes-1); l++)
		{
			h = 0;
			for(k=0; k<y_size; k++)
			{
				h = h + theta[l][k]*x[j][k];
			}
			sum = sum + fun_ex(h);
		}
	  
	  
		//Calculate probability of each class
		for(n=0; n<(classes-1); n++)
		{
			h_[n] = 0;
			for(k=0;k<y_size;k++)
			{
				h_[n] = h_[n] + theta[n][k]*x[j][k];
			}
			p_[n] = fun_ex(h_[n]) / sum;
			temp_sum = temp_sum + p_[n];
		}
		p_[classes] = 1 - temp_sum;

	
		//sort probability to find max-probability class
		for(n=0; n<classes; n++)
		{
			if(temp<p_[n])
			{
				class = n+1;
				temp = p_[n];
			}
		}
		
		//threshold selection
		result[j] = p_[class] > threshold ? class : 0;
	}

	// free malloc values
	free(h_);
	free(p_);

	return IMAGE_RET_OK;
}



