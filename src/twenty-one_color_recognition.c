#include "ALLDEFINE.h"


//============================================================================
//the only function that needs to be ported, applied to your own environment MCU and so on
//============================================================================
static void ReadColor(u16** image, u16 x, u16 y, COLOR_RGB *Rgb)
{
	unsigned int C16;

	C16 = image[y][x];                   //Image color at this point

	Rgb->red = (unsigned char)((C16 & 0xff0000) >> 16);
	Rgb->green = (unsigned char)((C16 & 0x00ff00) >> 8);
	Rgb->blue = (unsigned char)((C16 & 0x00ff));
}



//============================================================================
//RGB×ªHSL
//============================================================================
static void RGBtoHSL(const COLOR_RGB *Rgb, COLOR_HSL *Hsl)
{
	int h, s, l, maxVal, minVal, difVal;
	int r = Rgb->red;
	int g = Rgb->green;
	int b = Rgb->blue;

	maxVal = max3v(r, g, b);
	minVal = min3v(r, g, b);

	difVal = maxVal - minVal;

	//Calculate the brightness
	l = (maxVal + minVal) * 240 / 255 / 2;

	if (maxVal == minVal)//if r=g=b
	{
		h = 0;
		s = 0;
	}
	else
	{
		//Calculate is tonal
		if (maxVal == r)
		{
			if (g >= b)
				h = 40 * (g - b) / (difVal);
			else
				h = 40 * (g - b) / (difVal)+240;
		}
		else if (maxVal == g)
			h = 40 * (b - r) / (difVal)+80;
		else if (maxVal == b)
			h = 40 * (r - g) / (difVal)+160;
		//Calculate saturation
		if (l == 0)
			s = 0;
		else if (l <= 120)
			s = (difVal) * 240 / (maxVal + minVal);
		else
			s = (difVal) * 240 / (480 - (maxVal + minVal));
	}
	Hsl->hue = (unsigned char)(((h > 240) ? 240 : ((h < 0) ? 0 : h)));
	Hsl->saturation = (unsigned char)(((s > 240) ? 240 : ((s < 0) ? 0 : s)));
	Hsl->luminance = (unsigned char)(((l > 240) ? 240 : ((l < 0) ? 0 : l)));
}

//============================================================================
//match color
//============================================================================
static int ColorMatch(const COLOR_HSL *Hsl, const TARGET_CONDI *Condition)
{
	if (
		Hsl->hue > Condition->H_MIN &&
		Hsl->hue		<	Condition->H_MAX &&
		Hsl->saturation	>	Condition->S_MIN &&
		Hsl->saturation	<   Condition->S_MAX &&
		Hsl->luminance	>	Condition->L_MIN &&
		Hsl->luminance < Condition->L_MAX
		)
		return 1;
	else
		return 0;
}

//============================================================================
//Search for corrosion center
//============================================================================
static int SearchCentre(u16** image, u16 *x, u16 *y, const TARGET_CONDI *Condition, const SEARCH_AREA *Area)
{
	unsigned int SpaceX, SpaceY, i, j, k, FailCount = 0;
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;

	SpaceX = Condition->WIDTH_MIN / 3;
	SpaceY = Condition->HIGHT_MIN / 3;

	for (i = Area->Y_Start;i < Area->Y_End;i += SpaceY)
	{
		for (j = Area->X_Start;j < Area->X_End;j += SpaceX)
		{
			FailCount = 0;
			for (k = 0;k < SpaceX + SpaceY;k++)
			{
				if (k < SpaceX)
					ReadColor(image, j + k, i + SpaceY / 2, &Rgb);
				else
					ReadColor(image, j + SpaceX / 2, i + (k - SpaceX), &Rgb);
				RGBtoHSL(&Rgb, &Hsl);

				if (!ColorMatch(&Hsl, Condition))
					FailCount++;
				if (FailCount > ((SpaceX + SpaceY) >> ALLOW_FAIL_PER))
					break;
			}
			if (k == SpaceX + SpaceY)
			{
				*x = j + SpaceX / 2;
				*y = i + SpaceY / 2;
				return 1;
			}
		}
	}

	return 0;

}

//============================================================================
// Corrosion from the center outwards to obtain a new corrosion center
//============================================================================
static int Corrode(u16** image, u16 width, u16 height, unsigned int oldx, unsigned int oldy, const TARGET_CONDI *Condition, RESULT *Resu)
{
	unsigned int Xmin, Xmax, Ymin, Ymax, i, FailCount = 0;
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;

	for (i = oldx;i > 0;i--)
	{
		ReadColor(image, i, oldy, &Rgb);
		RGBtoHSL(&Rgb, &Hsl);
		if (!ColorMatch(&Hsl, Condition))
			FailCount++;
		if (FailCount > (((Condition->WIDTH_MIN + Condition->WIDTH_MAX) >> 2) >> ALLOW_FAIL_PER))
			break;
	}
	Xmin = i;
	FailCount = 0;

	for (i = oldx;i < 0 + width;i++)
	{
		ReadColor(image, i, oldy, &Rgb);
		RGBtoHSL(&Rgb, &Hsl);
		if (!ColorMatch(&Hsl, Condition))
			FailCount++;
		if (FailCount > (((Condition->WIDTH_MIN + Condition->WIDTH_MAX) >> 2) >> ALLOW_FAIL_PER))
			break;
	}
	Xmax = i;
	FailCount = 0;

	for (i = oldy;i > 0;i--)
	{
		ReadColor(image, oldx, i, &Rgb);
		RGBtoHSL(&Rgb, &Hsl);
		if (!ColorMatch(&Hsl, Condition))
			FailCount++;
		if (FailCount > (((Condition->HIGHT_MIN + Condition->HIGHT_MAX) >> 2) >> ALLOW_FAIL_PER))
			break;
	}
	Ymin = i;
	FailCount = 0;

	for (i = oldy;i < 0 + height;i++)
	{
		ReadColor(image, oldx, i, &Rgb);
		RGBtoHSL(&Rgb, &Hsl);
		if (!ColorMatch(&Hsl, Condition))
			FailCount++;
		if (FailCount > (((Condition->HIGHT_MIN + Condition->HIGHT_MAX) >> 2) >> ALLOW_FAIL_PER))
			break;
	}
	Ymax = i;
	FailCount = 0;

	Resu->x = (Xmin + Xmax) / 2;
	Resu->y = (Ymin + Ymax) / 2;
	Resu->w = Xmax - Xmin;
	Resu->h = Ymax - Ymin;

	if (((Xmax - Xmin) > (Condition->WIDTH_MIN)) && ((Ymax - Ymin) > (Condition->HIGHT_MIN)) && \
		((Xmax - Xmin) < (Condition->WIDTH_MAX)) && ((Ymax - Ymin) < (Condition->HIGHT_MAX)))
		return 1;
	else
		return 0;
}


/*
\\\\	Function name : image_Color_Trace
\\\\	Function uses : give the target Condition struct, it will return the RESULT struct which includes target x,y and width and height.
\\\\	Parameters : 
	    1> image : the detect image
		2> width : the search area width in image
		3> height : the search area height in image
		4> Area : SEARCH_AREA struct, includes image range format like {upleft.x, lowright.x, upleft.y, lowright.y} : SEARCH_AREA Area = { 0, 0 + width, 0, 0 + height };//识别图像范围 宽*高
		5> Condition : TARGET_CONDI struct, includes target conditions u want
		6> Resu : RESULT struct, the result it returns
		7> depth : the image must be RGB565 format
		8> ITERATE_NUM : The larger the number of iterations, the more accurate and time-consuming. usually set to 8
*/

u8 image_Color_Trace(u16** image, u16 width, u16 height, SEARCH_AREA *Area, const TARGET_CONDI *Condition, RESULT *Resu, int depth, int ITERATE_NUM)
{
	if (image == NULL || width < 1 || height < 1 || Area==NULL || Condition==NULL || Resu==NULL || depth != IM_24U)	return IMAGE_RET_ERROR;

	u16 i;
	static u16 x0, y0, flag_ = 0;
	RESULT Result;	                                        //save temp

	if (flag_ == 0)
	{
		if (SearchCentre(image, &x0, &y0, Condition, Area))   //search corrode center
			flag_ = 1;
		else
		{
			Area->X_Start = 0;
			Area->X_End = 0 + width;
			Area->Y_Start = 0;
			Area->Y_End = 0 + height;

			if (SearchCentre(image, &x0, &y0, Condition, Area))
			{
				flag_ = 0;
				return IMAGE_RET_ERROR;
			}
		}
	}
	Result.x = x0;
	Result.y = y0;

	for (i = 0;i < ITERATE_NUM; i++)
		Corrode(image, width, height, Result.x, Result.y, Condition, &Result);   //Corrosion from the center outwards to obtain a new corrosion center

	if (Corrode(image, width, height, Result.x, Result.y, Condition, &Result))
	{
		x0 = Result.x;
		y0 = Result.y;
		Resu->x = Result.x;
		Resu->y = Result.y;
		Resu->w = Result.w;
		Resu->h = Result.h;
		flag_ = 1;

		Area->X_Start = Result.x - ((Result.w) >> 1);
		Area->X_End = Result.x + ((Result.w) >> 1);
		Area->Y_Start = Result.y - ((Result.h) >> 1);
		Area->Y_End = Result.y + ((Result.h) >> 1);

		return IMAGE_RET_OK;
	}
	else
	{
		flag_ = 0;
		return IMAGE_RET_ERROR;
	}

}

//Using Example
//RESULT Resu;
//TARGET_CONDI Condition={60,100,20,120,10,160,40,40,320,240};
//SEARCH_AREA Area = {0, 0+width, 0, 0+height};
//int depth=IM_16U;
//int ITERATE_NUM=8;
//Trace(image, width, height, &Area, &Condition, &Resu, depth, ITERATE_NUM);


