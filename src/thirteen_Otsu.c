#include "ALLDEFINE.h"

static double dabs(double a, double b)
{
    if (a < b)
        return b-a;
    else
        return a-b;
}

static void calculate_histogram(u32 height, u32 width, u16** image, u32* histogram)
{
    u16 k;
    for(int i=0; i < height; i++){
        for(int j=0; j < width; j++){
            k = image[i][j];
            histogram[k] = histogram[k] + 1;
        }
    }
}

void calculate_pi(u32 height, u32 width, u32 histogram[], double pi[])
{
    for (int i = 0; i < GRAY_LEVELS; ++i){
        pi[i] = (double)histogram[i] / (double)(height * width);
    }
}

static double p1(int k, double pi[])
{
    double sum = 0.0;

    for (int i = 0; i <= k; i++){
        sum += pi[i];
    }

    return sum;
}

static double m(int k, double pi[])
{
    double sum = 0.0;

    for (int i = 0; i < k; i++){
        sum += i * pi[i];
    }

    return sum;
}

static double calculate_sigma(int k, double mg, double pi[])
{
    double p1k = p1(k, pi);
    double mk = m(k, pi);

    if (p1k < 1e-10 || (1 - p1k) < 1e-10)
        return 0.0;
    else
        return pow(mg * p1k - mk, 2) / (p1k * (1 - p1k));
}

/*	 otsu binary image	*/
u8 image_otsu(u8** in_array, u8** out_array, u32 height, u32 width, int depth)
{
	if( depth!=IM_8U || in_array==NULL || out_array == NULL || height<1 || width<1)	return IMAGE_RET_ERROR;
	
    u32 histogram[GRAY_LEVELS] = {0};
    double pi[GRAY_LEVELS] = {0};
    double sigma[GRAY_LEVELS] = {0};
    double mg=0;
    u16 max_count = 0;
    u16 k = 0;
    double max_value = 0.0;
    double k_star=0;

    calculate_histogram(height, width, in_array, histogram);
    calculate_pi(height, width, histogram, pi);
    mg = m(GRAY_LEVELS, pi);

    for (int i = 0; i < GRAY_LEVELS; i++)
        sigma[i] = calculate_sigma(i, mg, pi);

    max_value = sigma[0];
    max_count = 1;
    k = 0;
    for (int i = 1; i < GRAY_LEVELS; i++){
        if (dabs(sigma[i], max_value) < 1e-10){
            k += i;
            max_count++;
        }
        else if (sigma[i] > max_value){
            max_value = sigma[i];
            max_count = 1;
            k = i;
        }
    }
    k_star = k / max_count;

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (in_array[i][j] <= k_star)
                out_array[i][j] = 0x00;
            else
                out_array[i][j] = 0xff;
        }
    }
	
	return IMAGE_RET_OK;
}