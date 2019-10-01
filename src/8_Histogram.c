#include "ALLDEFINE.h"

static void calculate_histogram(u32 height, u32 width, u8 **image, u32 histogram[])
{
    u16 k;
    for(int i=0; i < height; i++){
        for(int j=0; j < width; j++){
            k = image[i][j];
            histogram[k] = histogram[k] + 1;
        }
    }
} 

/* histogram equalization */
u8 image_histogram_equalization(u8** in_array, u8** out_array, u32 height, u32 width, int depth)
{
	if (in_array == NULL || out_array == NULL || width < 1 || height < 1 || depth != IM_8U)	return IMAGE_RET_ERROR;

    u32 sum, sum_of_h[GRAY_LEVELS];
    double constant;
    u32 histogram[GRAY_LEVELS] = {0};

    calculate_histogram(height, width, in_array, histogram);
    sum = 0;
    for(int i=0; i < GRAY_LEVELS; i++){
        sum         = sum + histogram[i];
        sum_of_h[i] = sum;
    }

    constant = (double)(GRAY_LEVELS)/(double)(height*width);
    for(int i = 0, k = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            k = in_array[i][j];
            out_array[i][j] = sum_of_h[k] * constant;
        }
    }
	
	return IMAGE_RET_OK;
}