#include "ALLDEFINE.h"

static u8 is_in_array(u16 x, u16 y, u16 height, u16 width)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        return 1;
    else
        return 0;
}

/* 			Inverse mapping affine_transformation			*/
u8 image_affine_transformation(u16** in_array, u16 height, u16 width,
                            u16** out_array, int scale, int scalex, int scaley,
							int rotate, double angle, int translation, int transx, int transy,
							int vertical, int ver, int horizon, int hor, int depth)
{
	if (in_array == NULL || out_array == NULL || width < 1 || height < 1 || depth <= 0)	return IMAGE_RET_ERROR;

    u16  x1, y1, x2, y2, f11, f12, f21, f22;
    double x=0, y=0;

    for (int i = 0; i < height; i++)
	{
        for (int j = 0; j < width; j++)
		{
			if(scale)
			{
				if( scalex==0 || scaley ==0 )
				{
					return IMAGE_RET_ERROR;
				}
				else
				{
					x = j/scalex;
					y = i/scaley;
				}
			}
			
			if(rotate)
			{
				double cosangle = cos(angle);
				double sinangle = sin(angle);
				x = (j + i - y*(cosangle - sinangle) - height/2 - width/2) / (cosangle + sinangle);
				y = (i - j - x*(cosangle - sinangle) - height/2 + width/2) / (cosangle + sinangle);
			}
			
			if(translation)
			{
				x = j - transx;
				y = i - transy;
			}
			if(vertical)
			{
				x = (j-y) / ver;
				y = i;
			}
			if(horizon)
			{
				x = j;
				y = i - hor*x;
			}
			else
			{
				x = j;
				y = i;
			}
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