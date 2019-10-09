#ifndef __Harris_match_config_H
#define __Harris_match_config_H

#define MAX_CONERS 	200
#define HARRIS_TH   0.002f
#define SAMPLE		2

#define SCALE 		8		//ƥ��������С��EDGE
#define ERR_TH		0.1		//ƥ����ֵ

#define EDGE  			SCALE/SAMPLE		//ͼ��߽粻���ǵ�
#define IMAGE_WIDTH  	256	//ͼ���С
#define IMAGE_HEIGHT 	256
#define IMAGE_SIZE		IMAGE_WIDTH*IMAGE_HEIGHT
#define SAMPLE_W		IMAGE_WIDTH/SAMPLE
#define SAMPLE_H		IMAGE_HEIGHT/SAMPLE
#define SAMPLE_SIZE		SAMPLE_W*SAMPLE_H
#define INPUT_IMAGE_ADDRESS 	(0xC8000000) 	//����ͼ��ĵ�ַ����ͼ��洢Ϊint�ͣ�����512*512ͼ��
#define I2x_IMAGE_ADDRESS 		(0xC8100000)
#define I2y_IMAGE_ADDRESS 		(0xC8200000)
#define Ixy_IMAGE_ADDRESS 		(0xC8300000)

#define I2xR_IMAGE_ADDRESS 		(0xC8400000)	//���˲�������ݱ����ַ�������ã�ʵ�ʿ�����
#define I2yR_IMAGE_ADDRESS 		(0xC8500000)
#define IxyR_IMAGE_ADDRESS 		(0xC8600000)

#define G2x_IMAGE_ADDRESS 		(0xC8700000)
#define G2y_IMAGE_ADDRESS 		(0xC8800000)
#define Gxy_IMAGE_ADDRESS 		(0xC8900000)

#define R_IMAGE_ADDRESS 		(0xC8A00000)

#define IMAGE_ADDRESS			(0xC8B00000)
double H3[7] = { 0.0702, 0.1311, 0.1907, 0.2161, 0.1907, 0.1311, 0.0702 };
double H2[5] = { 0.1525, 0.2218, 0.2514, 0.2218, 0.1525};
#define NHCenter 3
#define Hsize NHCenter*2+1//5//7	//ͼ�����˴�С
double* pH = (double *)H3;			//��˹�˲�����ָ��
									//ָ����ָ��ַ��Ҫ���������˲��������޸�
//��ά��˹����ˣ�����sigma = 2
double H33[7][7] = { 	0.0049, 0.0092, 0.0134, 0.0152, 0.0134, 0.0092, 0.0049,
						0.0092, 0.0172, 0.0250, 0.0283, 0.0250, 0.0172, 0.0092,
						0.0134, 0.0250, 0.0364, 0.0412, 0.0364, 0.0250, 0.0134,
						0.0152, 0.0283, 0.0412, 0.0467, 0.0412, 0.0283, 0.0152,
						0.0134, 0.0250, 0.0364, 0.0412, 0.0364, 0.0250, 0.0134,
						0.0092, 0.0172, 0.0250, 0.0283, 0.0250, 0.0172, 0.0092,
						0.0049, 0.0092, 0.0134, 0.0152, 0.0134, 0.0092, 0.0049};

double H22[5][5] = { 	0.0232, 0.0338, 0.0383, 0.0338, 0.0232
						, 0.0338, 0.0492, 0.0558, 0.0492, 0.0338
						, 0.0383, 0.0558, 0.0632, 0.0558, 0.0383
						, 0.0338, 0.0492, 0.0558, 0.0492, 0.0338
						, 0.0232, 0.0338, 0.0383, 0.0338, 0.0232};

#endif