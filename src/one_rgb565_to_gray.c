#include "ALLDEFINE.h"

//         turn RGB565 to Gray
u8 image_rgb565togray(u16* src,u8* output,u16 Width,u16 Height)
{
	if (src==NULL || output==NULL || Width<1 || Height<1)	return IMAGE_RET_ERROR;

    u16 i,j,color565=0;
    u8 gray=0;

    for (i = 0; i < Height; i++)
    {
        gray++;
        for(j=0;j<Width;j++)
        {
            color565++;
            color565=*(src +i*Width+j);
            gray=(((color565&0xF800)>> 8)*76+((color565&0x7E0)>>3)*150+((color565&0x001F)<<3)*30)>>8;
            *(output+i*Width+j)=gray;
        }
    }

	return IMAGE_RET_OK;
}