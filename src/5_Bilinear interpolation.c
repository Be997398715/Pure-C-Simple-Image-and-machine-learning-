#include "ALLDEFINE.h"

static u8 is_in_array(u16 x, u16 y, u16 height, u16 width)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        return 1;
    else
        return 0;
}

/* bilinera interpolation resize */
u8 image_bilinera_interpolation(u16** in_array, u16 height, u16 width,
                            u16** out_array, u16 out_height, u16 out_width, int depth)
{
	if (in_array == NULL || out_array == NULL || width < 1 || height < 1 || out_height<1 || out_width<1 || depth<=0)	return IMAGE_RET_ERROR;

    double h_times = (double)out_height / (double)height,
           w_times = (double)out_width / (double)width;
    u16  x1, y1, x2, y2, f11, f12, f21, f22;
    double x, y;

    for (int i = 0; i < out_height; i++){
        for (int j = 0; j < out_width; j++){
            x = j / w_times;
            y = i / h_times;
            x1 = (u16)(x - 1);
            x2 = (u16)(x + 1);
            y1 = (u16)(y + 1);
            y2 = (u16)(y - 1);
            f11 = is_in_array(x1, y1, height, width) ? in_array[y1][x1] : 0;
            f12 = is_in_array(x1, y2, height, width) ? in_array[y2][x1] : 0;
            f21 = is_in_array(x2, y1, height, width) ? in_array[y1][x2] : 0;
            f22 = is_in_array(x2, y2, height, width) ? in_array[y2][x2] : 0;
            out_array[i][j] = (u16)(((f11 * (x2 - x) * (y2 - y)) +
                                     (f21 * (x - x1) * (y2 - y)) +
                                     (f12 * (x2 - x) * (y - y1)) +
                                     (f22 * (x - x1) * (y - y1))) 
									 / ((x2 - x1) * (y2 - y1)));
        }
    }
	
	return IMAGE_RET_OK;
}