#include "ALLDEFINE.h"

//          将RGB565格式的像素点提取出三通道RGB
void rgb565toRGB3(u16 rgb565,u8 *R,u8 *G,u8 *B)
{
    *R=((rgb565&0xF800)>> 8);//提取R通道
    *G=((rgb565&0x7E0)>>3);//提取G通道
    *B=((rgb565&0x001F)<<3);//提取B通道
}
//          将三通道RGB图的像素点转化为RGB565格式输出
u16 RGB3torgb565(u8 R,u8 G,u8 B)
{
                    // R                      G                      B
    u16 rgb565=((((u16)R)<<8)&0xF800)+((((u16)G)<<3)&0x7E0)+((((u16)B)>>3)&0x001F);
    return rgb565;
}

//          灰度图转彩色图
u16 graytorgb565(u8 gray)
{
                              // R                      G                      B
    u16 rgb565=((((u16)gray)<<8)&0xF800)+((((u16)gray)<<3)&0x7E0)+((((u16)gray)>>3)&0x001F);
    return rgb565;
}

//          将灰度格式的像素点转化为二值图输出
u8 graytobinary(u8 gray,u8 threshold)
{
    u8 binary;
    binary=gray>threshold?255:0;//二值化阈值分割
    return binary;
}
