#include "ALLDEFINE.h"

/* image corrosion */
u8 image_Corrosion(u8* in_array, u8* out_array, u32 height, u32 width, int depth, int** kernel, int kernel_size)
{
	if (depth != IM_8U || in_array == NULL || out_array == NULL || height < 1 || width < 1 || kernel == NULL || kernel_size %2==0 || kernel_size>1)	return IMAGE_RET_ERROR;
	
    int i, j, k, o;
	int size = (int)floor(kernel_size / 2);
    u8 flag;

    //针对图像中每一个像素位置，判断是否结构元素能填入目标内部
    for(i = 2; i < height-2; i++) {
        for(j = 2; j < width-2; j++) {        
            //判断结构元素是否可以在当前点填入目标内部，1为可以，0为不可以
            flag = 1;
            for(k = -size; k <= size && flag; k++) {
                for(o = -size; o <= size; o++) {
                    //如果当前结构元素位置为1，判断与对应图像上的像素点是否为非0
                    if(kernel[k+2][o+2]) {
                        //如果图像当前像素为0，则没有击中该点，不是腐蚀的输出
                        if(!*(in_array+(i+k)*height+width+o)){
                            flag = 0;    break;
                        }
                    }
                }
            }
            *(out_array+i*height+width) = flag ? 255 : 0;
        }
    }
	
	return IMAGE_RET_OK;
}

/* image Bi_Expansion */
u8 image_Bi_Expansion(u8* in_array, u8* out_array, u32 height, u32 width, int depth, int** kernel, int kernel_size)
{
	if (depth != IM_8U || in_array == NULL || out_array == NULL || height < 1 || width < 1 || kernel == NULL || kernel_size % 2 == 0 || kernel_size>1)	return IMAGE_RET_ERROR;

	int i, j, k, o;
	int size = (int)floor(kernel_size / 2);
    int flag;
	
    //膨胀是对图像中目标补集的腐蚀，因此先求输入图像数据的补集
    for(i = 0; i < height; i++)
        for(j = 0; j < width; j++)
            *(in_array+i*height+j) = 255 - *(in_array+i*height+j);
    //膨胀是结构元素的对称集对补集的腐蚀，此处求其反射
    for(i = 0; i < size; i++)
        for(j = 0; j <= i; j++)
			kernel[i][j] = kernel[j][i];


    //针对图像中每一个像素位置，判断是否结构元素能填入目标内部
    for(i = -size; i < height- size; i++) {
        for(j = size; j < width- size; j++) {
            //判断结构元素是否可以在当前点填入目标内部，1为可以，0为不可以
            flag = 1;
            for(k = -size; k <= size && flag; k++) {
                for(o = -size; o <= size; o++) {
                    //如果当前结构元素位置为1，判断与对应图像上的像素点是否为非0
                    if(kernel[k+2][o+2]) {
                        if(!*(in_array+(i+k)*height+j+o)){//没击中
                            flag = 0;    break;
                        }
                    }
                }
            }
            *(out_array+i*height+j) = flag ? 255 : 0;
        }
    }
    //用结构元素对称集对目标补集腐蚀后，还要对结构再求一次补集，才是膨胀结构输出
    //赋值结构元素腐蚀漏掉的区域，使原图像恢复为二值图像
    for(i = 0; i < height; i++) {
        for(j = 0; j < width; j++) {
            *(out_array+i*height+j) = 255 - *(out_array+i*height+j);
            if(*(out_array+i*height+j) != 255 && *(out_array+i*height+j) != 0)
                *(out_array+i*height+j) = 0;
        }
    }
	
	return IMAGE_RET_OK;
}