#include "ALLDEFINE.h"

/*
\\\\	Function name : image_templateMatch
\\\\	Function uses : give the target image template and search it in src image,  return the match point
\\\\	Parameters :
		1> src : the detect image
		2> temp : the search template
		3> match_degree : the return match degree, evaluate similarity
		4> i_match : the point y at src
		5> j_match : the point x at src
		6> Match_methold : the match function flag
		7> depth : the image must be gray format
*/
u8 image_templateMatch(const u8** src, u16 src_width, u16 src_height, const u8** temp, u16 temp_width, u16 temp_height, float* match_degree, u16* i_match, u16* j_match, int Match_methold, int depth)
{
	if (src == NULL || src_width < 1 || src_height < 1 || temp_width < 1 || temp_height < 1 || Match_methold<0 || Match_methold >1 || i_match == NULL || j_match == NULL || 
		temp_width>src_width || temp_height>src_height || depth != IM_8U)	return IMAGE_RET_ERROR;

	int i_end = src_height - temp_height + 1;
	int j_end = src_width - temp_width + 1;

	for (int i = 0; i < i_end; i++)
	{
		for (int j = 0; j < j_end; j++)
		{
			float match_ij = 0.0;

			for (int m = 0; m < temp_height; m++)
			{
				for (int n = 0; n < temp_width; n++)
				{
					u8 val_s = src[i + m][j + n];
					u8 val_t = temp[m][n];
					if (Match_methold == SQDIFF)
					{
						match_ij += (float)((val_t - val_s) * (val_t - val_s));
					}
					else if (Match_methold == SADIFF)
					{
						match_ij += (float)(abs(val_t - val_s));
					}
				}
			}

			//cout << match_ij << endl;
			if (match_ij < *match_degree)
			{
				*match_degree = match_ij;
				*i_match = i;
				*j_match = j;
			}
		}
	}


	return IMAGE_RET_OK;
}

//
//int main()
//{
//	Mat src = imread("messi5.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//	Mat temp = imread("face.png", CV_LOAD_IMAGE_GRAYSCALE);
//
//	float match_degree = 0;
//	int i_match = -1, j_match = -1;
//	float match_degree_max = 3.4E+38;

//	int ret = templateMatch(src, temp, &match_degree_max, &i_match, &j_match, SQDIFF);
//
//	cout << i_match << j_match << match_degree << endl;
//
//	Mat src_color;
//	cvtColor(src, src_color, COLOR_GRAY2BGR);
//	rectangle(src_color, Rect(j_match, i_match, temp.cols, temp.rows), Scalar(255, 0, 0));
//	imshow("result", src_color);
//	waitKey(0);
//
//	return 0;
//}
//


