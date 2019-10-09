#include "ALLDEFINE.h"

/* 待优化算法，O(n^4) */

u8 image_dft(u16** in_array, double** re_array, double** im_array, u16 height, u16 width, int depth)
{
	if (depth != IM_8U || width < 1 || height < 1 || width < 1 || in_array == NULL || re_array == NULL || im_array==NULL)	return IMAGE_RET_ERROR;

	double re, im, temp;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			re = 0;
			im = 0;

			for (int x = 0; x < height; x++) {
				for (int y = 0; y < width; y++) {
					temp = (double)i * x / (double)height +
						(double)j * y / (double)width;
					re += in_array[x][y] * cos(-2 * Pi * temp);
					im += in_array[x][y] * sin(-2 * Pi * temp);
				}
			}

			re_array[i][j] = re;
			im_array[i][j] = im;
		}
	}

	return IMAGE_RET_OK;
}



u8 image_fre_spectrum(u8 **in_array, u8 **out_array, u16 height, u16 width, int depth, int translation)
{
	if (depth != IM_8U || width < 1 || height < 1 || width < 1 || in_array == NULL || out_array == NULL)	return IMAGE_RET_ERROR;

    double re, im, temp;
    int move;

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            re = 0;
            im = 0;

            for (int x = 0; x < height; x++){
                for (int y = 0; y < width; y++){
                    temp = (double)i * x / (double)height + 
                           (double)j * y / (double)width;
                    move = (x + y) % 2 == 0 ? 1 : -1;
                    re += in_array[x][y] * cos(-2 * Pi * temp) * move;
                    im += in_array[x][y] * sin(-2 * Pi * temp) * move;
                }
            }
            
            out_array[i][j] = (short)(sqrt(re*re + im*im) / sqrt(width*height));
            if (out_array[i][j] > 0xff)
                out_array[i][j] = 0xff;
            else if (out_array[i][j] < 0)
                out_array[i][j] = 0;
		}
    }
	
	return IMAGE_RET_OK;
}



u8 image_idft(double** re_array, double** im_array, u8** out_array, u16 height, u16 width, int depth)
{
	if (depth != IM_8U || width<1 || height < 1 || width < 1 || re_array == NULL || im_array ==NULL || out_array == NULL)	return IMAGE_RET_ERROR;

    double real, temp;

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            real = 0;

            for (int x = 0; x < height; x++){
                for (int y = 0; y < width; y++){
                    temp = (double)i * x / (double)height + 
                           (double)j * y / (double)width;

                    real += re_array[x][y] * cos(2 * Pi * temp) -
                            im_array[x][y] * sin(2 * Pi * temp);
                }
            }
            
            out_array[i][j] = (u16)(real / sqrt(width*height));
            if (out_array[i][j] > 0xff)
                out_array[i][j] = 0xff;
            else if (out_array[i][j] < 0)
                out_array[i][j] = 0;
        }
    }
	
	return IMAGE_RET_OK;
}