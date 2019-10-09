#include "ALLDEFINE.h"


static u8 is_in_array(u16 x, u16 y, u16 height, u16 width)
{
	if (x >= 0 && x < height && y >= 0 && y < width)
		return 1;
	else
		return 0;
}

static void get_mean(LAB** src, float* mean_L, float* mean_a, float* mean_b, u16 height, u16 width)
{
	int i,j,temp = 0;
	float mean_L_temp=0, mean_a_temp=0, mean_b_temp=0;
	
	for (i = 0;i < height;i++)
	{
		for (j = 0;j < width;j++)
		{
			mean_L_temp = mean_L_temp + src[i][j].L;
			mean_a_temp = mean_a_temp + src[i][j].A;
			mean_b_temp = mean_b_temp + src[i][j].B;
		}
	}

	temp = height * width;
	*mean_L = mean_L_temp / temp;
	*mean_a = mean_a_temp / temp;
	*mean_b = mean_b_temp / temp;
}


static void calculate_salient_map(SRD** salient_map, u16** gf_image, float mean_L, float mean_a, float mean_b, u16 height, u16 width)
{
	int i, j;

	for (i = 0;i < height;i++)
	{
		for (j = 0;j < width;j++)
		{
			salient_map[i][j].R = gf_image[i][j] & 0xff0000 >> 16;
			salient_map[i][j].R = salient_map[i][j].R - mean_L;
			salient_map[i][j].G = gf_image[i][j] & 0xff00 >> 8;
			salient_map[i][j].G = salient_map[i][j].G - mean_a;
			salient_map[i][j].B = gf_image[i][j] & 0xff;
			salient_map[i][j].B = salient_map[i][j].B - mean_b;
		}
	}
}

static void min_max(float* array, int n, float* max_value, float* min_value)
{
	int i, maxIndex=0, minIndex=0;

	for (i = 1;i < n;i++)//从第一位开始，默认第一位最大，也最小
	{
		if (array[maxIndex] < array[i])//如果找到更大的
		{
			maxIndex = i;//记下更大的元素的下标
		}
		if (array[minIndex] > array[i])//如果找到更小的
		{
			minIndex = i;//记下更小的元素的下标
		}
	}
	*max_value = array[maxIndex];
	*min_value = array[minIndex];
}

static void normalize_salient_map(SRD** salient_map, u16 height, u16 width)
{
	int i, j;
	float max_value, min_value;

	//SRD two-dimension to one-dimension
	int k = 0;
	float* map = (float*)malloc(sizeof(SRD)*height*width);

	for (i = 0;i < height;i++)
	{
		for (j = 0;j < width;j++)
		{
			map[k++] = salient_map[i][j].R;
			map[k++] = salient_map[i][j].G;
			map[k++] = salient_map[i][j].B;
		}
	}
	min_max(map, k, &max_value, &min_value);

	//normalize
	float cha = max_value - min_value;
	for (i = 0;i < height;i++)
	{
		for (j = 0;j < width;j++)
		{
			salient_map[i][j].R = (salient_map[i][j].R - min_value) / cha;
			salient_map[i][j].G = (salient_map[i][j].G - min_value) / cha;
			salient_map[i][j].B = (salient_map[i][j].B - min_value) / cha;
		}
	}
	free(map);
}

static void float2int(const SRD** salient_map, u16** dst, u16 height, u16 width)
{
	int i,j;
	u16 temp1, temp2, temp3;

	for (i = 0;i < height;i++)
	{
		for (j = 0;j < width;j++)
		{
			temp1 = salient_map[i][j].R * 255;
			temp2 = salient_map[i][j].G * 255;
			temp3 = salient_map[i][j].B * 255;
			dst[i][j] = temp1 <<16 + temp2<<8 + temp3;
		}
	}
}

/*
\\\\ name : Gaussian_kernel
\\\\ function : generate two-dimension gaussian kernel
\\\\ parameters : 
	 1> kernel：the gaussian kernel
	 2> size：  kernel size, usually is 3 or 5
	 3> sigma： Standard deviation of a normal distribution, the bigger sigma is, the image blur more
*/
u8 image_Gaussian_kernel(float **kernel, int size, float sigma)
{
	if (kernel == NULL || size <= 0 || sigma == 0)	return IMAGE_RET_ERROR;

	int x, y;
	int m = size / 2;
	float sum = 0;

	//get kernel
	for (y = 0; y < size; y++)
	{
		for (x = 0; x < size; x++)
		{
			kernel[y][x] = (1 / (2 * Pi * sigma * sigma)) * exp(-((x - m) * (x - m) + (y - m) * (y - m)) / (2 * sigma * sigma));
			sum += kernel[y][x];
		}
	}

	//normal
	for (y = 0; y < size; y++)
	{
		for (x = 0; x < size; x++)
		{
			kernel[y][x] /= sum;
		}
	}

	return IMAGE_RET_OK;
}


/*
\\\\ name : image_Gaussian_filter
\\\\ function : filter image using gaussian kernel
\\\\ parameters :
	 1> src : image to be filtered
	 2> dst : image  be filtered
	 3> height : image height
	 4> width : image width
	 5> kernel : gaussian kernel
	 6> size：  kernel size, usually is 3 or 5
*/
u8 image_Gaussian_filter(u16**src, u16**dst, u16 height, u16 width, float** kernel, int size)
{
	if ( src==NULL || dst==NULL || kernel == NULL || height <= 0 || width <= 0 || size==0)	return IMAGE_RET_ERROR;

	int y, x;
	int i, j;
	int m = size / 2;
	float value;
	int temp;
	float temp1;

	float *kernel_vec = (float*)malloc(sizeof(float)*size*size);

	//kernel two dimensions to one dimension
	int k = 0;
	for (j = 0; j < size; j++)
	{
		for (i = 0; i < size; i++)
		{
			kernel_vec[k++] = kernel[j][i];
		}
	}

	//gaussian convolution, the boundary is padding zero
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			value = 0.0f;
			k = 0;
			for (j = -m; j < m;j++)
			{
				for (i = -m; i < m; i++)
				{
					temp = is_in_array(y + j, x + i, height, width) ? src[y+j][x+i] : 0;
					temp1 = kernel_vec[k++];
					value += temp * temp1;
				}
			}
			dst[y][x] = value;
		}
	}

	free(kernel_vec);

	return IMAGE_RET_OK;
}



u8 FT_SRDetection(u16*src, u16*dst, u16 height, u16 width, int size, float sigma, int depth)
{
	if (src == NULL || dst == NULL || height <= 0 || width <= 0 || depth!=IM_24U)	return IMAGE_RET_ERROR;

	//1.The image img is filtered by gaussian to get gf_image
	float** kernel = (float**)malloc(sizeof(float)*size);
	for (int i = 0;i < size;i++)
	{
		kernel[i] = (float**)malloc(sizeof(float)*size);
	}
	u16** gf_image = (u16*)malloc(sizeof(u16)*height);
	for (int i = 0;i < size;i++)
	{
		gf_image[i] = (float**)malloc(sizeof(float)*width);
	}

	printf("start gaussian kernel\r\n");
	image_Gaussian_kernel(kernel, size, sigma);
	printf("start gaussian filter\r\n");
	image_Gaussian_filter(src, gf_image, height, width, kernel, size);


	//2.Convert image imgrgb from RGB color space to LAB color space imglab
	printf("start image_RGB2Lab\r\n");
	LAB** imglab = (LAB**)malloc(sizeof(LAB)*height);
	for (int i = 0; i < height; i++)
	{
		imglab[i] = (LAB*)malloc(sizeof(LAB)*width);
	}
	image_RGB2Lab(src, height, width, imglab, IM_24U);


	//3.The images of L, A and B channels of the converted image imglab were averaged respectively to obtain lm, am and bm
	printf("start get_mean LAB\r\n");
	float L_mean, A_mean, B_mean;
	get_mean(imglab, &L_mean, &A_mean, &B_mean, height, width);

	//4.Calculate the significant value, that is, take the Euclidean distance and sum the average image of three channels and the image obtained by filtering
	printf("start calculate_salient_map\r\n");
	SRD** imgsrd = (SRD**)malloc(sizeof(SRD)*height);
	for (int i = 0; i < height; i++)
	{
		imgsrd[i] = (SRD*)malloc(sizeof(SRD)*width);
	}
	calculate_salient_map(imgsrd, gf_image, L_mean, A_mean, B_mean, height, width);

	//5.The maximum value is used to normalize the significant graph and Convert float to int for show if u want
	printf("start normalize_salient_map\r\n");
	normalize_salient_map(imgsrd, height, width);
	float2int(imgsrd, dst, height, width);
	
	

	// 释放开辟的二维数组空间
	for (int i = 0; i < height; ++i)
	{
		free(*(imglab + i));
	}

	for (int i = 0; i < height; ++i)
	{
		free(*(imgsrd + i));
	}
	for (int i = 0; i < size; ++i)
	{
		free(*(kernel+i));
	}

	for (int i = 0; i < height; ++i)
	{
		free(*(gf_image + i));
	}

	return IMAGE_RET_OK;
}




//
//#include "ALLDEFINE.h"
//#include "opencv2/opencv.hpp"
//#include <iostream>
//using namespace std;
//using namespace cv;
//
//Mat Vec2Mat(u16 **array, int row, int col);
//void Mat2Vec(Mat mat, u16** array);
//
//
//int main()
//{
//	printf("This is My Image Lib test.\r\n");
//
//	Mat img = imread("Demo.jpg");
//
//	u16 **src;
//	src = (u16 **)malloc(sizeof(u16)*img.rows);
//	for (int i = 0; i < img.rows; i++)
//	{
//		//printf("srci=%d\r\n",i);
//		src[i] = (u16 *)malloc(sizeof(u16)*img.cols);
//	}
//	if (src == NULL) {
//		printf("src malloc fail\r\n");
//	}
//	else {
//		printf("src malloc successful\r\n");
//	}
//
//	u16 **dst;
//	dst = (u16 **)malloc(sizeof(u16)*img.rows);
//	for (int i = 0; i < img.rows; i++)
//	{
//		//printf("dst i=%d\r\n", i);
//		dst[i] = (u16 *)malloc(sizeof(u16)*img.cols);
//	}
//	if (dst == NULL) {
//		printf("dst malloc fail\r\n");
//	}
//	else {
//		printf("dst malloc successful\r\n");
//	}
//
//	//imshow("src", img);
//	//waitKey(0);
//
//	cvtColor(img, img, CV_BGR2RGB);
//	Mat2Vec(img, src);
//	printf("mat2vec successful\r\n");
//
//
//	//SRDetection
//	cout << "start FT-SDR!" << endl;
//	FT_SRDetection(src, dst, img.rows, img.cols, 5, 1.0, IM_24U);
//
//	Mat output = Vec2Mat(dst, img.rows, img.cols);
//	imshow("dst", output);
//	waitKey(0);
//
//
//	// 释放开辟的二维数组空间
//	for (int i = 0; i < img.rows; ++i)
//	{
//		free(*(src + i));
//	}
//
//	for (int i = 0; i < img.rows; ++i)
//	{
//		free(*(dst + i));
//	}
//
//	getchar();
//
//	return 0;
//}
//
//
//
//
//Mat Vec2Mat(u16 **array, int row, int col)
//{
//	Mat img(row, col, CV_8UC3);
//
//	for (int i = 0; i < row; i++)
//	{
//		for (int j = 0; j < col; j++)
//		{
//			img.at<Vec3b>(i, j)[0] = array[i][j] & 0xff0000 >> 16;	//R
//			img.at<Vec3b>(i, j)[1] = array[i][j] & 0x00ff00 >> 8;	//G
//			img.at<Vec3b>(i, j)[2] = array[i][j] & 0xff;			//B
//		}
//	}
//
//	return img;
//}
//
//
//void Mat2Vec(Mat mat, u16** array)
//{
//	for (int i = 0; i < mat.rows; i++)
//	{
//		for (int j = 0; j < mat.cols; j++)
//		{
//			//printf("%d,%d,%d\r\n", mat.at<Vec3b>(i, j)[0], mat.at<Vec3b>(i, j)[1], mat.at<Vec3b>(i, j)[2]);
//			array[i][j] = ((u16)mat.at<Vec3b>(i, j)[0] << 16) + ((u16)mat.at<Vec3b>(i, j)[1] << 8) + ((u16)mat.at<Vec3b>(i, j)[2]);		//R+G+B
//		}
//	}
//
//}