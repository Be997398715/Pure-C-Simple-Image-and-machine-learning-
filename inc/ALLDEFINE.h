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

#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))

/*		struct type  	*/
typedef struct{
    float x;
    float y;
}image_Point;

typedef struct             //Criteria for determination as an objective
{
	unsigned char  H_MIN;    //Target minimum tone
	unsigned char  H_MAX;    //Target maximun tone	

	unsigned char  S_MIN;    //Target minimum saturation
	unsigned char  S_MAX;    //Target maximum saturation

	unsigned char  L_MIN;    //Target minimum brightness
	unsigned char  L_MAX;    //Maximum target brightness

	unsigned int   WIDTH_MIN; //Target minimum width
	unsigned int   HIGHT_MIN; //Target minimum height

	unsigned int   WIDTH_MAX; //Target maximum width
	unsigned int   HIGHT_MAX; //Target maximum height
}TARGET_CONDI;

typedef struct             //recognition result
{
	unsigned int x;         //target X point
	unsigned int y;         //target Y point
	unsigned int w;         //target width
	unsigned int h;         //target height
}RESULT;

typedef struct                      //RGB format color
{
	unsigned char  red;             // [0,255]
	unsigned char  green;           // [0,255]
	unsigned char  blue;            // [0,255]
}COLOR_RGB;

typedef struct                      //HSL format color
{
	unsigned char hue;              // [0,240]
	unsigned char saturation;       // [0,240]
	unsigned char luminance;        // [0,240]
}COLOR_HSL;

typedef struct                      //area
{
	unsigned int X_Start;
	unsigned int X_End;
	unsigned int Y_Start;
	unsigned int Y_End;
}SEARCH_AREA;

typedef struct                      //LAB
{
	float L;
	float A;
	float B;
}LAB;

typedef struct                      //SRD
{
	float R;
	float G;
	float B;
}SRD;



/*		flag type  	*/
#define IM_8U 1
#define IM_24U 2
#define IM_24S 3
#define IM_32F 4


#define IMAGE_RET_ERROR -1	//图像处理失败标志
#define IMAGE_RET_OK 0		//图像处理成功标志
	


/*		functions   	*/
#ifdef __cplusplus
extern "C" {
#endif
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


	/*			FFT2D	To be optimized		*/
	u8 image_dft(u16** in_array, double** re_array, double** im_array, u16 height, u16 width, int depth);
	u8 image_fre_spectrum(u8 **in_array, u8 **out_array, u16 height, u16 width, int depth, int translation);
	u8 image_idft(double** re_array, double** im_array, u8** out_array, u16 height, u16 width, int depth)


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
	u8 image_k_means(image_Point* point, image_Point* mean, int N, int k, int* center, int verbose, float threshold);


	/*			gray/binary/float image/matrix distance transform		*/
	//gray/binary/float image/matrix dt
	u8 image_dt(double *m, int rows, int cols, int depth);
	//gray image/matrix dt
	u8 image_dt_gray(u8 *gray, int rows, int cols, int step, int depth);
	//binary image/matrix dt
	u8 image_dt_binary(u8 *bimg, int rows, int cols, int step, int depth);


	/*						Color recognition				*/
	#define ALLOW_FAIL_PER 3   //Fault tolerance rate: every 1<<ALLOW_FAIL_PER point allows an error point to occur. 
					   //The higher the fault tolerance rate is, the easier it is to identify, but the higher the error rate is.
	u8 image_Color_Trace(u16** image, u16 width, u16 height, SEARCH_AREA *Area, const TARGET_CONDI *Condition, RESULT *Resu, int depth, int ITERATE_NUM);


	/*						template matching				*/
	#define SQDIFF 0	// match flag
	#define SADIFF 1
	u8 image_templateMatch(const u8** src, u16 src_width, u16 src_height, const u8** temp, u16 temp_width, u16 temp_height, float* match_degree, u16* i_match, u16* j_match, int Match_methold, int depth);


	/*						Image thin algorithm				*/
	//Hilditch thin algorithm
	u8 image_ThinnerHilditch(void *image, u16 lx, u16 ly, int depth);
	//Pavlidis thin algorithm
	u8 image_ThinnerPavlidis(void *image, u16 lx, u16 ly, int depth);
	//Rosenfeld thin algorithm
	u8 image_ThinnerRosenfeld(void *image, u16 lx, u16 ly, int depth);


	/*					 Image RGB--LAB						*/
	//image rgb2lab
	u8 image_RGB2Lab(const int** rgb_img, u16 height, u16 width, LAB** lab_img, int depth);
	//image lab2rgb
	u8 image_Lab2RGB(const LAB** lab_img, u16 height, u16 width, int** rgb_img, int depth);



	/*					 Image Salient Region Detection						*/
	//image FT-SDR
	u8 FT_SRDetection(u16** src, u16** dst, u16 height, u16 width, int size, float sigma, int depth);
	//image Gaussian filter
	u8 image_Gaussian_filter(u16** src, u16** dst, u16 height, u16 width, float** kernel, int size);
	//image Gaussian_kernel generate
	u8 image_Gaussian_kernel(float **kernel, int size, float sigma);


#ifdef __cplusplus
	}
#endif


#endif