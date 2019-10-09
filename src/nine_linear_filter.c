#include "ALLDEFINE.h"

static u8 is_in_array(u16 x, u16 y, u16 height, u16 width)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        return 1;
    else
        return 0;
}

/*	3*3 linear fitler */
u8 image_linear_filter(u8** in_array, u8** out_array, u16 height, u16 width, int depth)
{
	if (in_array == NULL || out_array == NULL || width < 1 || height < 1 || depth != IM_8U)	return IMAGE_RET_ERROR;

    u16 value[9];
	u8 ARRAY_SIZE = 9;
	u8 average[3][3] = {{1,2,1}, {2,4,2}, {1,2,1}};
	
	
    /* linear filtering */
    u16 sum=0;
    for (int i = 0; i < ARRAY_SIZE; i++)
	{
        for (int j = 0; j < ARRAY_SIZE; j++)
		{
            sum += average[i][j];
		}
	}

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            value[0] = is_in_array(j-1, i-1, height, width) ? in_array[i-1][j-1] : 0;
            value[1] = is_in_array(j, i-1, height, width) ? in_array[i-1][j] : 0;
            value[2] = is_in_array(j+1, i-1, height, width) ? in_array[i-1][j+1] : 0;
            value[3] = is_in_array(j-1, i, height, width) ? in_array[i][j-1] : 0;
            value[4] = in_array[i][j];
            value[5] = is_in_array(j+1, i, height, width) ? in_array[i][j+1] : 0;
            value[6] = is_in_array(j-1, i+1, height, width) ? in_array[i+1][j-1] : 0;
            value[7] = is_in_array(j, i+1, height, width) ? in_array[i+1][j] : 0;
            value[8] = is_in_array(j+1, i+1, height, width) ? in_array[i+1][j+1] : 0;

            /* linear filtering */
            out_array[i][j] = (value[0] * average[0][0] + value[1] * average[0][1] + value[2] * average[0][2] +
                               value[3] * average[1][0] + value[4] * average[1][1] + value[5] * average[1][2] +
                               value[6] * average[2][0] + value[7] * average[2][1] + value[8] * average[2][2]) / sum;

        }
    }
	
	return IMAGE_RET_OK;
}