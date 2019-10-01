#include "ALLDEFINE.h"

//          image dyeing
u8 image_dyeing(u8* gray_pic,u16* rgb,u16 Width,u16 Height,u8 rR,u8 rG,u8 rB, int depth)
{
	if (gray_pic == NULL || rgb == NULL || Width < 1 || Height < 1 || depth != IM_8U)	return IMAGE_RET_ERROR;

    float R,G,B;
    u16 i,j,color565;
    u8 gray;

    R=(rR/255.0) ;
    G=(rG/255.0);
    B=(rB/255.0);
    for (i = 0; i < Height; i++)
    {
         for(j=0;j<Width;j++)
         {
             gray=*(gray_pic+i*Width+j);
             color565=RGB3torgb565((u8)(R*gray),(u8)(G*gray),(u8)(B*gray));
             *(rgb+i*Width+j)=color565;
         }
	}

	return IMAGE_RET_OK;
}