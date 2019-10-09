#include "ALLDEFINE.h"

//RGB2Lab Lab2RGB
const float param_13 = 1.0f / 3.0f;
const float param_16116 = 16.0f / 116.0f;
const float Xn = 0.950456f;
const float Yn = 1.0f;
const float Zn = 1.088754f;
 
static void RGB2XYZ(u8 R, u8 G, u8 B, float *X, float *Y, float *Z)
{
	 *X = 0.412453f * R + 0.357580f * G + 0.180423f * B;
	 *Y = 0.212671f * R + 0.715160f * G + 0.072169f * B;
	 *Z = 0.019334f * R + 0.119193f * G + 0.950227f * B;
}
 
static void XYZ2Lab(float X, float Y, float Z, float *L, float *a, float *b)
{
	float fX, fY, fZ;
#if 1
	X /= (255.0*Xn);
	Y /= (255.0*Yn);
	Z /= (255.0*Zn);
#endif
 
 
	if (Y > 0.008856f)
	{
		fY = pow(Y, param_13);
		*L = 116.0f * fY -16.0f;
	}
	else
	{
		fY = 7.787f * Y + param_16116;
		*L = 903.3*fY;
	}
	
	
	*L = *L > 0.0f ? *L : 0.0f;
	
	if (X > 0.008856f)
		fX = pow(X, param_13);
	else
		fX = 7.787f * X + param_16116;
	
	if (Z > 0.008856)
		fZ = pow(Z, param_13);
	else
		fZ = 7.787f * Z + param_16116;
 
	*a = 500.0f * (fX - fY);
	*b = 200.0f * (fY - fZ);
}

/*
\\\\ Function name : image_RGB2Lab
\\\\ Function explain : turn RGB888 image to LAB space format
\\\\ Function Parameters : 
	 1> rgb_img : RGB888 format image, order as R-G-B
	 2> height : image height
	 3> width : image width
	 4> lab_img : LAB struct format data
	 5> depth : only need IM_24U data
*/
u8 image_RGB2Lab(const int** rgb_img, u16 height, u16 width, LAB** lab_img, int depth)
{
	if (rgb_img == NULL || width < 1 || height < 1 || lab_img==NULL || depth != IM_24U)	return IMAGE_RET_ERROR;

	int temp;
	float X,Y,Z,L,a,b;
	u32 line_byte;
	u16 i,j;
	
	for (i = 0; i <height;i++)
	{
		for (j = 0;j < width;j++)
		{
			temp = rgb_img[i][j];

			//high eight bits represent R
			//middle eight bits represent G
			//low eight bits represent B
			RGB2XYZ( temp&0xff0000>>16, temp&0xff00>>8, temp&0xff, &X, &Y, &Z);
			XYZ2Lab(X,Y,Z,&L,&a,&b);
 
			lab_img[i][j].L = L;
			lab_img[i][j].A = a;
			lab_img[i][j].B = b;
		}
	}
    
	return IMAGE_RET_OK;
}

 
static void XYZ2RGB(float X, float Y, float Z, u8 *R, u8 *G, u8 *B)
{
	float RR, GG, BB;
	RR = 3.240479f * X - 1.537150f * Y - 0.498535f * Z;
	GG = -0.969256f * X + 1.875992f * Y + 0.041556f * Z;
	BB = 0.055648f * X - 0.204043f * Y + 1.057311f * Z;
 
	if(RR<0.0)
	{
		RR = 0;
	}else if(RR>255.0)
	{
		RR = 255;
	}else
	{
		RR = (u8)RR;
	}
 
 
	if(GG<0.0)
	{
		GG = 0;
	}else if(GG>255.0)
	{
		GG = 255;
	}
	else
	{
		GG = (u8)GG;
	}
 
 
	if(BB<0.0)
	{
		BB = 0;
	}else if(BB>255.0)
	{
		BB = 255;
	}
	else
	{
		BB = (u8)BB;
	}
	
	*R = (u8)RR;
	*G = (u8)GG;
	*B = (u8)BB;
 }
 
static void Lab2XYZ(float L, float a, float b, float *X, float *Y, float *Z)
{
	float fX, fY, fZ;
	
	fY = (L + 16.0f) / 116.0f;
	if (fY > 0.206893f)
		*Y = fY * fY * fY;
	else
		*Y = (fY-param_16116) / 7.787f;
	
	fX = a / 500.0f + fY;
	if (fX > 0.206893f)
		*X = fX * fX * fX;
	else
		*X = (fX-param_16116) / 7.787f;
	
		fZ = fY - b / 200.0f;
	if (fZ > 0.206893f)
		*Z = fZ * fZ * fZ;
	else
		*Z = (fZ -param_16116) / 7.787f;
 
#if 1
	(*X) *= (0.950456f*255.0);
	(*Y) *= (255.0*1.0);
	(*Z) *= (1.088754f*255.0);
#endif
}


/*
\\\\ Function name : image_Lab2RGB
\\\\ Function explain : turn LAB space format to RGB888 image 
\\\\ Function Parameters :
	 1> lab_img : LAB struct format data
	 2> height : image height
	 3> width : image width
	 4> rgb_img : RGB888 format image, order as R-G-B
	 5> depth : only need IM_24U data
*/
u8 image_Lab2RGB(const LAB** lab_img, u16 height, u16 width, int** rgb_img, int depth)
{
	if (rgb_img == NULL || width < 1 || height < 1 || lab_img == NULL || depth != IM_24U)	return IMAGE_RET_ERROR;

	u8 R,G,B;
	float X,Y,Z,L,a,b;
	u16 i,j;
	
	for (i = 0; i <height;i++)
	{
		for (j = 0;j < width;j++)
		{
			L = lab_img[i][j].L;
			a = lab_img[i][j].A;
			b = lab_img[i][j].B;

			Lab2XYZ(L,a,b,&X,&Y,&Z);
			XYZ2RGB(X,Y,Z,&R,&G,&B);
		
			rgb_img[i][j] = R<<16 + G<<8 + B;
		}
	}
 
	return IMAGE_RET_OK;
}