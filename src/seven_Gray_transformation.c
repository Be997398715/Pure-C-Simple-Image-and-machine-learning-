#include "ALLDEFINE.h"

/* image reverse */
u8 image_reverse(u8** in_array, u8** out_array, u16 height, u16 width, int depth)
{
	if (in_array == NULL || out_array == NULL || width < 1 || height < 1 || depth != IM_8U)	return IMAGE_RET_ERROR;

    for (int i = 0; i < height; i++){
        for (int j = 0; j <width; j++)
            out_array[i][j] = GRAY_LEVELS - 1 - in_array[i][j];
    }
	
	return IMAGE_RET_OK;
}

/* image logarithm */
u8 image_logarithm(u8** in_array, u8** out_array, u16 height, u16 width, int depth)
{
	if (in_array == NULL || out_array == NULL || width < 1 || height < 1 || depth != IM_8U)	return IMAGE_RET_ERROR;

    for (int i = 0; i < height; i++){
        for (int j = 0; j <width; j++)
            out_array[i][j] = (u16)(10 * log((1 + in_array[i][j])));
    }
	
	return IMAGE_RET_OK;
}
/* image gamma transform */
u8 image_gamma(u8** in_array, u8** out_array, u16 height, u16 width, float gamma, int depth)
{
	if (in_array == NULL || out_array == NULL || width < 1 || height < 1 || gamma <0 || depth != IM_8U)	return IMAGE_RET_ERROR;

    for (int i = 0; i < height; i++){
        for (int j = 0; j <width; j++)
            out_array[i][j] = (u16)pow(in_array[i][j], gamma);
    }
	
	return IMAGE_RET_OK;
}