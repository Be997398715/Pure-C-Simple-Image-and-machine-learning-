#include "ALLDEFINE.h"

//       gray to binary image
u8 pic_graytobinary(u8* src,u8* output,u8 threshold,u16 Width,u16 Height, int depth)
{
	if (src == NULL || output == NULL || Width < 1 || Height < 1 || depth != IM_8U || threshold<0)	return IMAGE_RET_ERROR;

    u8 gray;
    u16 i,j;
    for (i = 0; i < Height; i++)
    {
         for(j=0;j<Width;j++)
         {
             gray=*(src +i*Width+j);
             *(output+i*Width+j)=graytobinary(gray,threshold);
         }
    }
	
	return IMAGE_RET_OK;
}