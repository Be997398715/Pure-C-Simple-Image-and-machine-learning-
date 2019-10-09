#include "ALLDEFINE.h"
#include "Harris_match_config.h"

extern int Image_data1[];
extern int Image_data2[];

//角点
/*坐标，尺度，编号*/
typedef struct ConerPoint	
{
    int x, y, id;			
}ConerPoint;
ConerPoint Coner1[MAX_CONERS],Coner2[MAX_CONERS];


//匹配上的角点
/*匹配上的点坐标*/
/*描述符距离*/
/*序号*/
typedef struct MatchCorner
{
	double x1, y1, x2, y2;
	double distance;	
	int id;	
}MatchCorner;					
MatchCorner matchedCorner[MAX_CONERS];



static void Sample(int* src, int* dst, int w, int h,int ratio)
{
	int i,j;
	int* pDst = (int *)dst;
	int* pSrc = (int *)src;
	for(j=0;j<h;j+=ratio)
	{
		pSrc = (int *)(src+w*j);
		for(i=0;i<w;i+=ratio)
		{
			*pDst++ = *pSrc;
			pSrc += ratio;
		}
	}

}

/*******************************************************************************
 * Harris算法实现
 * 参数 src：图像首地址，coner:获取到的角点，th：harris阈值，w图像宽度，h图像高度
 * *****************************************************************************/
int Harris(int* src, ConerPoint* coner, double th, int w, int h)
{
	int N_Harris = 0;	//Harris角点个数
	int* pData = (int*)(src);			//INPUT_IMAGE_ADDRESS;
	double* pR = (double*)R_IMAGE_ADDRESS;
	int* pI2x = (int*)I2x_IMAGE_ADDRESS;
	int* pI2y = (int*)I2y_IMAGE_ADDRESS;
	int* pIxy = (int*)Ixy_IMAGE_ADDRESS;
	double* pI2xR = (double*)I2xR_IMAGE_ADDRESS;
	double* pI2yR = (double*)I2yR_IMAGE_ADDRESS;
	double* pIxyR  = (double*)IxyR_IMAGE_ADDRESS;
	double* pG2x = (double*)G2x_IMAGE_ADDRESS;
	double* pG2y = (double*)G2y_IMAGE_ADDRESS;
	double* pGxy = (double*)Gxy_IMAGE_ADDRESS;
	double temp_G2x, temp_G2y, temp_Gxy;
	double temp_R, max_R = 0.1f;
	int i,j,ii,temp_Ri,temp_Rj;
	int up_pix, down_pix, left_pix, right_pix;

	double *ptmpR;

	for(j=0;j<h;j++)
	{
		for(i=0;i<w;i++)
		{
			//else 中间区域
			left_pix = (int)*(pData - 1);
			right_pix = (int)*(pData + 1);

			//左边界，左像素点为0
			if(i==0)
			{
				left_pix = 0;
				right_pix = (int)*(pData+1);
			}
			//右边界，右侧像素点为0
			if( i==(w-1) )
			{
				left_pix = (int)*(pData-1);right_pix = 0;
			}

			//else 中间区域
			up_pix = *(pData - w);
			down_pix = *(pData + w);
			//上边界
			if(j==0)
			{
				up_pix = 0;
				down_pix = *(pData+ w);
			}
			//下边界
			if( j==(h-1) )
			{
				up_pix = *(pData- w);
				down_pix = 0;
			}

			*pI2x++ = (right_pix - left_pix)*(right_pix - left_pix);	//(*Ix)*(*Ix);
			*pI2y++ = (down_pix - up_pix)*(down_pix - up_pix);			//(*Iy)*(*Iy);
			*pIxy++ = (right_pix - left_pix)*(down_pix - up_pix);		//(*Ix)*(*Iy);
			pData++;

		}
	}

	pI2x = (int*)I2x_IMAGE_ADDRESS;		//指回首地址，以备滤波计算
	pI2y = (int*)I2y_IMAGE_ADDRESS;
	pIxy = (int*)Ixy_IMAGE_ADDRESS;
	//行滤波
	for(j=0;j<h;j++)
	{
		for(i=0;i< w;i++)
		{
			temp_G2x = 0.0f; temp_G2y = 0.0f; temp_Gxy = 0.0f;
			for(ii=0;ii<Hsize;ii++)
			{
				if( (i+ii-NHCenter>=0)&&(i+ii-NHCenter<w))
				{
					temp_G2x += *( pI2x+(ii-NHCenter) ) * (*(pH+ii));
					temp_G2y += *( pI2y+(ii-NHCenter) ) * (*(pH+ii));
					temp_Gxy += *( pIxy+(ii-NHCenter) ) * (*(pH+ii));
				}
			}
			*pI2xR++ = temp_G2x;	//注意I2x是整形，I2xR是double，这里发生了数据格式的变化
			*pI2yR++ = temp_G2y;
			*pIxyR++ = temp_Gxy;
			pI2x++;
			pI2y++;
			pIxy++;
		}
	}


	pI2xR = (double*)I2xR_IMAGE_ADDRESS;	//指回行滤波后的数据首地址，进行列滤波
	pI2yR = (double*)I2yR_IMAGE_ADDRESS;
	pIxyR = (double*)IxyR_IMAGE_ADDRESS;
	//列滤波
	for(j=0;j<h;j++)
	{
		for(i=0;i<w;i++)
		{
			temp_G2x = 0.0f; temp_G2y = 0.0f; temp_Gxy = 0.0f;
			for(ii=0;ii<Hsize;ii++)
			{
				if( (j+ii-NHCenter>=0)&&(j+ii-NHCenter<h) )
				{
						temp_G2x += *( pI2xR+(ii-NHCenter)*w ) * (*(pH+ii));
						temp_G2y += *( pI2yR+(ii-NHCenter)*w ) * (*(pH+ii));
						temp_Gxy += *( pIxyR+(ii-NHCenter)*w ) * (*(pH+ii));
				}
			}

			temp_R =  ( temp_G2x*temp_G2y - temp_Gxy*temp_Gxy ) - 0.06*(temp_G2x+temp_G2y)*(temp_G2x+temp_G2y);
			//找到R的最大值
			if( (temp_R-max_R)>0.5 )	
			{
				max_R = temp_R;
				temp_Ri = i;
				temp_Rj = j;
			}
			*pR++ = temp_R;
			*pG2x++ = temp_G2x;
			*pG2y++ = temp_G2y;
			*pGxy++ = temp_Gxy;
			pI2xR++;
			pI2yR++;
			pIxyR++;
		}
	}
	printf("max R = %.2f,(%d, %d)\n",max_R, temp_Rj+1 , temp_Ri+1);


	pR = (double*)R_IMAGE_ADDRESS;		//指回R首地址，寻找角点
	for(j=1;j<h-1;j++)
	{
		for(i=1;i<w-1;i++)
		{
			ptmpR = pR+j*w+i;
			temp_R = *ptmpR;

			//3*3邻域内非极大值抑制,不考虑图像边界上的点
			if( temp_R>(th*max_R) && N_Harris<MAX_CONERS )
			{
				if( (temp_R>*(ptmpR-1)) &&
					(temp_R>*(ptmpR+1)) &&
					(temp_R>*(ptmpR-w)) &&
					(temp_R>*(ptmpR-w-1)) &&
					(temp_R>*(ptmpR-w+1)) &&
					(temp_R>*(ptmpR+w)) &&
					(temp_R>*(ptmpR+w-1)) &&
					(temp_R>*(ptmpR+w+1)) && (i+1 > EDGE)  && (j+1 > EDGE) && (i+1 < w-EDGE) && (j+1 <h-EDGE)
				)
				{

					coner[N_Harris].x = i+1;
					coner[N_Harris].y = j+1;
					coner[N_Harris].id = ++N_Harris;
				}
			}/*end if((temp_R>(0.2*max_R))&&(N_Harris<MAX_CONERS))*/
		}/*end for i*/
	}/*end for j*/


	return N_Harris;
}

//寻找double型数组中最小的数，返回该最小值所在的位置
static int getMin(double *data, int number)
{
	int i, idx = 0;
	double temp = 0x7fffffff;
	for (i = 0; i<number; i++)
	{
		if (data[i]<temp)
		{
			temp = data[i];
			idx = i;
		}
	}
	return idx;
}


//返回匹配上的角点
int HarrisMatches(int* src1, int* src2, ConerPoint* c1, ConerPoint* c2, int nc1, int nc2, int w, int h)
{
	int i,j,k,t,r,l;
	int nMatched = 0;
	int Region[2*SCALE+1][2*SCALE+1] = {0};
	//double sum[MAX_CONERS] = { FLT_MAX };
	double dist[MAX_CONERS] = { 0x7fffffff };
	int suma;
	for(i=0; i<nc1; i++)
	{
		for(j=0 ; j<nc2; j++)
		{
			dist[j] = 0;
			suma = 0;
			for(r=0;r<2*SCALE+1;r++)
			{
				for(l=0;l<2*SCALE+1;l++)
				{
					Region[r][l] = src1[(c1[i].y-1+r-SCALE)*w + (c1[i].x-1+l-SCALE)] - src2[(c2[j].y-1+r-SCALE)*w + (c2[j].x-1+l-SCALE)];
					if(Region[r][l] < 0)	
						Region[r][l] = -Region[r][l];
					dist[j] += Region[r][l];
					suma += src1[(c1[i].y-1+r-SCALE)*w + (c1[i].x-1+l-SCALE)];
				}
			}
			dist[j] = dist[j] / (double)suma;
		}
		j = getMin(dist, nc2);
		if(dist[j]<=ERR_TH) 
			nMatched++;
		for (k = 0; k < i; k++)
		{
			if(dist[j] < matchedCorner[k].distance)	
				break;
		}
		for (t = i; t > k; t--)
		{
			matchedCorner[t].id = matchedCorner[t - 1].id;
			matchedCorner[t].x1 = matchedCorner[t - 1].x1;
			matchedCorner[t].x2 = matchedCorner[t - 1].x2;
			matchedCorner[t].y1 = matchedCorner[t - 1].y1;
			matchedCorner[t].y2 = matchedCorner[t - 1].y2;
			matchedCorner[t].distance = matchedCorner[t - 1].distance;
		}
		matchedCorner[t].id = j;			//id记录匹配到的点在特征点多的特征集合中的位置
		matchedCorner[t].x1 = c1[i].x;
		matchedCorner[t].y1 = c1[i].y;
		matchedCorner[t].x2 = c2[j].x;
		matchedCorner[t].y2 = c2[j].y;
		matchedCorner[t].distance = dist[j];
	}

	return nMatched;
}



//int main(void) {
//	int i,n1,n2,nMatched;
//
//	int* mydata = (int*)IMAGE_ADDRESS;
//	printf("Harris start: Sample = %d , Region = %d\n", SAMPLE, SCALE);
//	
//	Sample((int *)Image_data1, mydata, IMAGE_WIDTH, IMAGE_HEIGHT, SAMPLE);
//	//start = clock();
//	n1 = Harris( (int *)mydata,  Coner1, HARRIS_TH , SAMPLE_W, SAMPLE_H);
//	//stop=clock();
//	//printf("the cycle to get %d corner1 is %d\n",n1,stop-start);
//	for(i=0;i<n1;i++)
//	{
//		Coner1[i].x *= SAMPLE;
//		Coner1[i].y *= SAMPLE;
//	}
//	//for(i=0;i<n1;i++)
//	//	printf("%d Harris: %3d,%3d\n",Coner1[i].id,Coner1[i].x,Coner1[i].y);
//
//	Sample((int *)Image_data2, mydata, IMAGE_WIDTH, IMAGE_HEIGHT, SAMPLE);
//	//start = clock();
//	n2 = Harris( (int *)mydata,  Coner2, HARRIS_TH , SAMPLE_W, SAMPLE_H); 	//n2图像2角点个数
//	//stop=clock();
//	//printf("the cycle to get %d corner2 is %d\n",n2,stop-start);
//
//	for(i=0;i<n2;i++)
//	{
//		Coner2[i].x *= SAMPLE;
//		Coner2[i].y *= SAMPLE;
//	}
//	//for(i=0;i<n2;i++)
//	//	printf("%d Harris: %3d,%3d\n",Coner2[i].id,Coner2[i].x,Coner2[i].y);
//	//start = clock();
//	nMatched =	HarrisMatches((int *)Image_data1,(int *)Image_data2, Coner1, Coner2, n1, n2, IMAGE_WIDTH, IMAGE_HEIGHT);
//	//stop=clock();
//	//printf("the cycle to match %d+%d corners are: %d\n",n1,n2,stop-start);
//
//	printf("Matched points: %d\n", nMatched);
//		for(i=0;i<n1;i++)		//按照距离从小到大输出匹配特征点
//			printf("%2d: matchedCorner[%2d] (%3.0f,%3.0f,%3.0f,%3.0f) d: %.5f\n", i, matchedCorner[i].id, matchedCorner[i].x1, matchedCorner[i].y1, matchedCorner[i].x2, matchedCorner[i].y2, matchedCorner[i].distance);
//
//	printf("Harris end!\n");
//	return 0;
//}
