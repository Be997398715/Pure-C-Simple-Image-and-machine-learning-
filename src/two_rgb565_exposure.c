#include "ALLDEFINE.h"

//          RGB565 image exposure 
u8 image_exposure(u16* src,u16* output,u16 Width,u16 Height)
{
	if (src == NULL || output == NULL || Width < 1 || Height < 1)	return IMAGE_RET_ERROR;

    u16 i,j,color565;
    u8 R,G,B;
    for (i = 0; i < Height; i++)
    {
        for(j=0;j<Width;j++)
        {
			color565 = *(src + i * Width + j);
            rgb565toRGB3(color565,&R,&G,&B);
            R=R>128?R:~R;
            G=G>128?G:~G;
            B=B>128?B:~B;
            *(output+i*Width+j)=RGB3torgb565(R,G,B);
        }
    }

	return IMAGE_RET_OK;
}
