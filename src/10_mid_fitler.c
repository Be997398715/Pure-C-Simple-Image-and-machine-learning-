#include "ALLDEFINE.h"

static u16 mid_val(u16* a, u16 num)
{
    u16 temp;
    for (int i = 0; i < num; i++)
    {
        temp = a[i];
        int j = i;
        for (; j > 0 && a[j - 1] > temp; j--)
            a[j] = a[j - 1];
        a[j] = temp;
    }

    return a[num/2];
}

static u8 is_in_array(u16 x, u16 y, u16 height, u16 width)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        return 1;
    else
        return 0;
}

/*	3*3 midean filter	*/
u8 image_mid_filter(u8** in_array, u8** out_array, u16 height, u16 width, u16 ksize, int depth)
{
	if(depth!=IM_8U || ksize % 2 == 0 || ksize == 1 || ksize > height || ksize > width || in_array==NULL || out_array==NULL)	return IMAGE_RET_ERROR;

    u16* value = (u16*)malloc(sizeof(u16)*ksize*ksize);

	u16 mid = ksize*ksize/2;
	
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
			for(int k = 0;k<ksize*ksize;k++)
			{
				int temp = (k-mid);
				int j_ = temp / ksize;
				int i_ = temp % ksize;
				value[k] = is_in_array(j+j_, i+i_, height, width) ? in_array[i][j-1] : 0;
			}
            /* median filtering */
            out_array[i][j] = mid_val(value, ksize*ksize);

        }
    }
	
	return IMAGE_RET_OK;
}