#include "ALLDEFINE.h"


//calculate euclid distance
static float getdistance(image_Point point1, image_Point point2)
{
    float d;
    d = sqrt((point1.x - point2.x)*(point1.x - point2.x) + (point1.y - point2.y)*(point1.y - point2.y));
    return d;
}

//cluster function
static void cluster(image_Point* point, image_Point* mean, int* center, float** distance, int N, int k, int verbose)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < k; j++)
        {
            distance[i][j] = getdistance(point[i], mean[j]);
        }
        float min = 9999.0;
        for (int j = 0; j < k; j++)
        {
            if (distance[i][j] < min)
            {
                min = distance[i][j];
                center[i] = j;
            }
        }
		if(verbose)		printf("(%.0f,%.0f)\t in cluster-%d\n", point[i].x, point[i].y, center[i] + 1);
    }
}

//calculate error function
static float gete(image_Point* point, image_Point* mean, int* center, int N, int k)
{
    float cnt=0, sum=0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (center[i] == j)
            {
                cnt = getdistance(point[i], mean[j]);
            }
        }
        sum += cnt;     
    }
    return sum;
}

//get cluster mean centern
static void getmean(image_Point* point, image_Point* mean, int* center, int verbose, int N, int k)
{
	image_Point sum;
    int count;
    for (int i = 0; i < k; i++)
    {
        sum.x = 0.0;
        sum.y = 0.0;
        count = 0;
        for (int j = 0; j < N; j++)
        {
            if (center[j] == i)
            {
                sum.x += point[j].x;
                sum.y += point[j].y;
                count++;
            }
        }
        mean[i].x = sum.x / count;
        mean[i].y = sum.y / count;
    }
	if(verbose)
	{
		for (int i = 0; i < k; i++)
		{
			printf("the new center point of %d is:\t(%f,%f)\n", i + 1, mean[i].x, mean[i].y);
		}
	}
} 


/*
\\\\ Function name: k-means
\\\\ Parameters: 
	 1> point : Point type data, the cluster data, the number if N
	 2> mean : Point type data, the cluster mean , the number if k
	 3> N : the data number
	 4> k : the cluster number
	 5> center : int type data, the data center, the number if N
	 6> verbose : whether u like to show training information
	 7> threshold : decide whether the iteration should be stopped
*/ 
u8 image_k_means(image_Point* point, image_Point* mean, int N, int k, int* center, int verbose, float threshold)
{
	if(point==NULL || mean==NULL || center==NULL || N<1 || k<0)	return IMAGE_RET_ERROR;
	
	float** distance = (float**)malloc(sizeof(float)*N*k);
	
    int number=0;
    float temp1, temp2;
	
	// rand seed for point mean
	srand(1001);
	for(int i=0; i<k; i++)
	{
		mean[i].x = rand() % 10 + 1;
		mean[i].y = rand() % 10 + 1;
	}


    //first cluster
    cluster(point, mean, center, distance, N, k, verbose);
    number++;
	
    //get the first cluster error function
    temp1 = gete(point, mean, center, N, k);
    printf("the error1 is:%f\n", temp1);
	
    //restart calculate cluster center
    getmean(point, mean, center, verbose, N, k);
	
    //second cluster
    cluster(point, mean, center, distance, N, k, verbose);
    number++;
    temp2 = gete(point, mean, center, N, k);
	if(verbose)		printf("the error2 is:%f\n", temp2);

    //Iteration except the absolute value > threshold
    while (fabs(temp1 - temp2) > threshold)
    {
        temp1 = temp2;
		getmean(point, mean, center, verbose, N, k);
        cluster(point, mean, center, distance, N, k, verbose);
        temp2 = gete(point, mean, center, N, k);
        number++;
		if(verbose)
		{
			printf("the error%d is:%f\n", number,temp2);
		}
    }
	
	if(verbose)
	{
		printf("the total number of cluster is:%d\n", number);
	}
	
	free(distance);
	
	return IMAGE_RET_OK;
}




/*
int main()
{
	int N=11;
	int k=3;

	Point point[N] = {
			{ 2.0, 10.0 }, { 2.0, 5.0 }, { 8.0, 4.0 }, { 5.0, 8.0 }, {7.0,5.0},
			{ 6.0, 4.0 }, { 1.0, 2.0 }, { 4.0, 9.0 }, { 7.0, 3.0 }, { 1.0, 3.0 }, {3.0,9.0}
	};
	int center[N];
	Point mean[k];
	
	k_means(point, mean, N, k, center, verbose=1);
}
*/