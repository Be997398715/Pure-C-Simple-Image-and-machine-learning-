#ifndef __ALLDEFINE_H
#define __ALLDEFINE_H

/*		library  	*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <stdint.h>

/*		data type  	*/
#define u8 	unsigned char
#define u16	unsigned int
#define u32 unsigned long

#define Pi 3.1415926535

#define GRAY_LEVELS 256

/*		struct type  	*/
typedef struct{
    float x;
    float y;
}Point;


/*		flag type  	*/
#define IM_8U 1
#define IM_16U 2
#define IM_16S 3
#define IM_32F 4

#define IMAGE_RET_ERROR -1	//图像处理失败标志
#define IMAGE_RET_OK 0		//图像处理成功标志
	


/*		functions   	*/

/*		pixel functions  	*/
void rgb565toRGB3(u16 rgb565, u8 *R, u8 *G, u8 *B);
u16 RGB3torgb565(u8 R, u8 G, u8 B);
u16 graytorgb565(u8 gray);
u8 graytobinary(u8 gray, u8 threshold);


/*      rgb565 to gray image   */
u8 image_rgb565togray(u16* src, u8* output, u16 Width, u16 Height);


/*      RGB565 image exposure    */
u8 image_exposure(u16* src, u16* output, u16 Width, u16 Height);


/*          image dyeing		*/
u8 image_dyeing(u8* gray_pic, u16* rgb, u16 Width, u16 Height, u8 rR, u8 rG, u8 rB, int depth);


/*      gray to binary image	*/
u8 pic_graytobinary(u8* src, u8* output, u8 threshold, u16 Width, u16 Height, int depth);


/*		bilinera interpolation resize	 */
u8 image_bilinera_interpolation(u16** in_array, u16 height, u16 width,
	u16** out_array, u16 out_height, u16 out_width, int depth);


/* 			Inverse mapping affine_transformation			*/
u8 image_affine_transformation(u16** in_array, u16 height, u16 width,
	u16** out_array, int scale, int scalex, int scaley,
	int rotate, double angle, int translation, int transx, int transy,
	int vertical, int ver, int horizon, int hor, int depth);


/*			gray transformation			*/
// image reverse transform
u8 image_reverse(u8** in_array, u8** out_array, u16 height, u16 width, int depth);
// image logarithm transform
u8 image_logarithm(u8** in_array, u8** out_array, u16 height, u16 width, int depth);
// image gamma transform
u8 image_gamma(u8** in_array, u8** out_array, u16 height, u16 width, float gamma, int depth);


/*			histogram equalization		*/
u8 image_histogram_equalization(u8** in_array, u8** out_array, u32 height, u32 width, int depth);


/*			3*3 linear fitler		 */
u8 image_linear_filter(u8** in_array, u8** out_array, u16 height, u16 width, int depth);


/*			3*3 midean filter		*/
u8 image_mid_filter(u8** in_array, u8** out_array, u16 height, u16 width, u16 ksize, int depth);


/*			3*3	sobel filter	*/
u8 image_sobel_filter(u8** in_array, u8** out_array, u16 height, u16 width, int depth, int dx, int dy, double scale);


/*			FFT2D			*/





/*			otsu binary image		*/
u8 image_otsu(u8** in_array, u8** out_array, u32 height, u32 width, int depth);


/*			binary image shape deal		*/
// image corrosion
u8 image_Corrosion(u8* in_array, u8* out_array, u32 height, u32 width, int depth, int** kernel, int kernel_size);
// image Bi_Expansion 
u8 image_Bi_Expansion(u8* in_array, u8* out_array, u32 height, u32 width, int depth, int** kernel, int kernel_size);


/*			KNN for 1d and 2d data		*/		
u8 image_Knn(u8* test_array, u8** train_array, u8* train_label, int train_size, u8* output_label, double* output_distance, u32 img_size, int K, int distance_flag, int depth);


/*			linear regression for 1d and 2d data	*/
u8 image_linear_regression(int *x, int *y, int size, float *cost, float learning_rate, int num_iterations, float *w, float *b);


/*			logistic regression for 1d and 2d data of two classes		*/
u8 image_logistic_regression(double ** x, int * y, double* theta, int x_size, int y_size, float learning_rate, int num_iterations);


/*			softmax regression for 1d and 2d data of many classes		*/
// softmax training
u8 image_softmax_regression(double** x, int* y, double** theta, int x_size, int y_size, int classes, float learning_rate, int num_iterations, int verbose);
// softmax predict
u8 image_predict_softmax_regression(int ** x, int* result, int x_size, int y_size, double** theta, int classes, double threshold);


/*			k_means 2d data of k classes		*/
u8 image_k_means(Point* point, Point* mean, int N, int k, int* center, int verbose, float threshold);



#endif