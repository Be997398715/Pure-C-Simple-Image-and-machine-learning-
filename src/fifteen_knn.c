#include "ALLDEFINE.h"

// Find the element in the array that appears the most
static u8 nearest_num(u8* a, u8* b, u8 n)
{
	int temp=0;
	
	for(int i=0;i<n;i++)
	{
		for(int j=i;j<n;j++)
		{
			if(a[i]==a[j])
				b[i]=b[i]+1;
		}
	}

	for(int i=1;i<n;i++)
	{
		if(b[temp]<b[i])
		{
			temp=i;
		}
	}
	return a[temp];
}

// The array is sorted from smallest to largest
static void sortLowtoHigh(double* distance, int size, u8* label)
{
    int i, j, temp, isSorted;
    int label_temp;
	
    //优化算法：最多进行 n-1 轮比较
    for(i=0; i<size-1; i++){
        isSorted = 1;  //假设剩下的元素已经排序好了
        for(j=0; j<size-1-i; j++){
            if(distance[j] > distance[j+1]){
				// tansfer distance element
                temp = distance[j];
                distance[j] = distance[j+1];
                distance[j+1] = temp;
				
				// tansfer label element too
                label_temp = label[j];
                label[j] = label[j+1];
                label[j+1] = label_temp;
				
                isSorted = 0;  //一旦需要交换数组元素，就说明剩下的元素没有排序好
            }
        }
        if(isSorted) break; //如果没有发生交换，说明剩下的元素已经排序好了
    }
}

// calculate euclid distance
static double distance_euclid(u8* point1, u8* point2, u32 img_size)
{
	double dist = 0.0, tmp = 0.0;
	int a;
	for(a=0; a<img_size; a++) {
		tmp = point2[a] - point1[a];
		tmp = tmp * tmp;
		dist += tmp;
	}
	dist = sqrt(dist);
	
	return dist;
}

// calculate cosine distance
static double similarity_cosine(u8* point1, u8* point2, u32 img_size)
{
	double sim = 0.0, tmp1 = 0.0, tmp2 = 0.0, tmp3 = 0.0;
	int a = 0;
	
	for(a=0; a<img_size; a++) {
		tmp1 += point1[a] * point1[a];
		tmp2 += point2[a] * point2[a];
		tmp3 += point1[a] * point2[a];
	}
	tmp1 = sqrt(tmp1);
	tmp2 = sqrt(tmp2);
	
	sim = tmp3 / (tmp1 * tmp2);
	
	return sim;
}

/*
\\\\\ function name: Knn 
\\\\\ function effect: using Knn to find which train_array is closest to test_array, and return test_array's predict label and distance
\\\\\ function paramters:
	  1> test_array: data which going to predict
	  2> train_array: data which is the database, for calculate and find the nearest data
	  3> train_label: the train_array's label
	  4> train_size: the numbers of train_array
	  5> output_label: the predict label of test_array
	  6> output_distance: the predict distance of test_array to the k-nearest train_array
	  7> img_size: the data size 
	  8> K: the number of the k-neares data
	  9> distance_flag: `1` using euclid distance, while `2` using cosine distance 
	  8> depth: the img depth, which is supported equal to `IM_8U`
*/

u8 image_Knn(u8* test_array, u8** train_array, u8* train_label, int train_size, u8* output_label, double* output_distance, u32 img_size, int K, int distance_flag, int depth)
{
	if( depth!=IM_8U || train_size<1 || img_size<1 ||(distance_flag>0 && distance_flag<3) || K>0)	return IMAGE_RET_ERROR;
	
	double* test_to_train_distance = (double*)malloc(sizeof(double)*train_size);
	double temp=0.0;

	u8* K_label = (u8*)malloc(sizeof(u8)*K);
	u8* K_temp = (u8*)malloc(sizeof(u8)*K);

	int K_sum=0;
	
	for(int idx_train=0; idx_train<train_size; idx_train++) {
		if(distance_flag==1)
			// using euclidean distance
			test_to_train_distance[idx_train] = distance_euclid(test_array, train_array[idx_train], img_size);
		if(distance_flag==2)
			// using consine similarity
			test_to_train_distance[idx_train] = similarity_cosine(test_array, train_array[idx_train], img_size);
	}
	
	sortLowtoHigh(test_to_train_distance, train_size, train_label);
	
	for(int i = 0; i<K; i++)
	{
		K_label[i] = train_label[i];
	}
	
	*output_label = nearest_num(K_label, K_temp, K);
	
	for(int i=0;i<K;i++)
	{
		if(K_label[i]==*output_label)
		{
			*output_distance = test_to_train_distance[i];
		}
		else;
	}
	
	free(test_to_train_distance);
	free(K_label);
	free(K_temp);
	
	return IMAGE_RET_OK;
}



/*
int main ()
{
	
	int SIZE_TRAIN = 6;
	int SIZE_TEST = 3;
	int SIZE_ATTR = 49;

	u8 train_data[SIZE_TRAIN][SIZE_ATTR];
	u8 train_class[SIZE_TRAIN];

	u8 train0[SIZE_ATTR] = {0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0};
	u8 train1[SIZE_ATTR] = {0,0,1,1,1,0,0,0,1,1,0,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,1,0,1,1,0,0,0,1,1,1,0,0};
	u8 train2[SIZE_ATTR] = {0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0};
	u8 train3[SIZE_ATTR] = {0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0};
	u8 train4[SIZE_ATTR] = {0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,1,0,0};
	u8 train5[SIZE_ATTR] = {0,1,1,1,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,1,1,0};
	
	u8 test0[SIZE_ATTR] = {0,0,1,1,1,0,0,1,1,1,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,1,1,1,0,0};

	memcpy(train_data[0], train0, SIZE_ATTR);
	memcpy(train_data[1], train1, SIZE_ATTR);
	memcpy(train_data[2], train2, SIZE_ATTR);
	memcpy(train_data[3], train3, SIZE_ATTR);
	memcpy(train_data[4], train4, SIZE_ATTR);
	memcpy(train_data[5], train5, SIZE_ATTR);
	
	train_class[0] = 0;
	train_class[1] = 0;
	train_class[2] = 1;
	train_class[3] = 1;
	train_class[4] = 2;
	train_class[5] = 2;

	u8 output_label;
	double output_distance;
	
	Knn(test0, train_data, train_class, SIZE_TRAIN, &output_label, &output_distance, SIZE_ATTR);
	
	printf("the test label = %d\r\n the test score = %.2f\r\n", output_label, output_distance);
}
*/