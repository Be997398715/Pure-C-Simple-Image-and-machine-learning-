#include "ALLDEFINE.h"

static int sum_val(int* value, u8 ksize, int** template, int flag)
{
    int temp = 0;
	if(flag==1)
	{
		for (int i = 0; i < ksize*ksize; i++)
		{
			temp = temp + value[i]*template[i/ksize][i%ksize];
		}
	}
	
    return temp;
}

static u8 mean_val(u8** in_array, u16 height, u16 width)
{
    u32 temp = 0;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
			temp = temp + in_array[i][j];
		}
	}
	
    return (u8)temp/(height*width);
}

static u8 is_in_array(u16 x, u16 y, u16 height, u16 width)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        return 1;
    else
        return 0;
}

/*	3*3	sobel filter	*/
u8 image_sobel_filter(u8** in_array, u8** out_array, u16 height, u16 width, int depth, int dx, int dy, double scale)
{
	if (depth != IM_8U || scale<0 || height<1 || width<1 || in_array == NULL || out_array == NULL)	return IMAGE_RET_ERROR;


	int templatex[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	int templatey[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

	int value[3*3] = {0};
	int temp_grad = 0;
	u16 mid = 3*3/2;
	
	// self-adaptive threshold hold calculate
	float threshold = 0;
	threshold = scale * mean_val(in_array,  height,  width);
	
	
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
			 /* grad sobel filter */
			for(int k = 0;k<3*3;k++)
			{
				int temp = (k-mid);
				int j_ = temp / 3;
				int i_ = temp % 3;
				value[k] = is_in_array(j+j_, i+i_, height, width) ? in_array[i+i_][j+j_] : 0;
			}
			
            /*  calculate dx and dy and gradient  */ 
			temp_grad = abs( sum_val(value, 3, templatex, dx) ) + abs( sum_val(value, 3, templatey, dy) );
			
			/*     edge enhancement : NMS   */
			if (temp_grad < 0)
                out_array[i][j] = 0;
            else if (temp_grad > 255)
                out_array[i][j] = 255;
            else if (temp_grad > threshold)
                out_array[i][j] = (u8)temp_grad;
            else
                out_array[i][j] = in_array[i][j];
        }
    }
	
	return IMAGE_RET_OK;
}