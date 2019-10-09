#include "ALLDEFINE.h"

#define INF 1E20

#define SQUARE(q) ((q)*(q))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define ROUND(t) ((int)((t) + 0.5))
#define BOUND_8U(t) ((t) < 0 ? 0 : (t) > 255 ? 255 : (t))

static void dt_row(const double *f, int n, double *d, double *z, int *v) 
{
	int k, q;

	v[0] = 0;
	z[0] = -INF;
	z[1] = +INF;

	k = 0;
	for (q = 1; q < n; ++q) {
		double s = ((f[q] + SQUARE(q)) - (f[v[k]] + SQUARE(v[k]))) / (double)(2 * q - 2 * v[k]);
		while (s <= z[k]) {
			k--;
			s = ((f[q] + SQUARE(q)) - (f[v[k]] + SQUARE(v[k]))) / (double)(2 * q - 2 * v[k]);
		}
		k++;
		v[k] = q;
		z[k] = s;
		z[k + 1] = +INF;
	}

	k = 0;
	for (q = 0; q < n; ++q) {
		while (z[k + 1] < q)
			k++;
		d[q] = SQUARE(q - v[k]) + f[v[k]];
	}
}

/*
\\\\ Function name: dt
\\\\ Parameters:
	 1> m : gray or binary or floating image/matrix
	 2> rows : matrix/image height
	 3> cols : matrix/image width
	 4> depth : image/matrix dimension

\\\\ DL : Squared Euclidean distance, referred to http://cs.brown.edu/~pff/dt/
*/

u8 image_dt(double *m, int rows, int cols, int depth) 
{
	if (m == NULL || rows < 1 || cols < 1 || depth != IM_8U)	return IMAGE_RET_ERROR;

	const int n = MAX(rows, cols);
	double *f = (double*)malloc(sizeof(f[0]) * n);
	double *d = (double*)malloc(sizeof(d[0]) * n);
	double *z = (double*)malloc(sizeof(z[0]) * (n + 1));
	int *v = (int*)malloc(sizeof(v[0]) * n);
	int x, y;

	for (x = 0; x < cols; ++x) {
		for (y = 0; y < rows; ++y) {
			f[y] = m[y*cols + x];
		}
		dt_row(f, rows, d, z, v);
		for (y = 0; y < rows; ++y) {
			m[y*cols + x] = d[y];
		}
	}

	for (y = 0; y < rows; ++y) {
		for (x = 0; x < cols; ++x) {
			f[x] = m[y*cols + x];
		}
		dt_row(f, cols, d, z, v);
		for (x = 0; x < cols; ++x) {
			m[y*cols + x] = d[x];
		}
	}

	free(f);
	free(d);
	free(z);
	free(v);


	return IMAGE_RET_OK;
}



static void min_max(const double *m, int sz, double *min, double *max) 
{
	double mi = m[0], ma = m[0];
	int i = 1;
	for (; i < sz; ++i) {
		if (m[i] > ma) {
			ma = m[i];
		}
		else if (m[i] < mi) {
			mi = m[i];
		}
	}
	*min = mi;
	*max = ma;
}

static void double_to_image(const double *m, int rows, int cols, u8 *data, int step) 
{
	int i, j;
	double mi, ma, scale;
	min_max(m, rows * cols, &mi, &ma);

	if (mi == ma) {
		return;
	}

	scale = 255.0 / (ma - mi);

	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			const double s = m[i*cols + j] * scale;
			const int t = ROUND(s);
			data[i*step + j] = BOUND_8U(t);
		}
	}
}

static void sqrt_m(double *m, int sz) 
{
	int i = 0;
	for (; i < sz; ++i) {
		m[i] = sqrt(m[i]);
	}
}



u8 image_dt_gray(u8 *gray, int rows, int cols, int step, int depth)
{
	if (gray == NULL || rows < 1 || cols <1 || step < 1 || depth!= IM_8U)	return IMAGE_RET_ERROR;

	double *m = (double*)malloc(sizeof(m[0]) * rows * cols);
	int i, j;
	const double vstep = 100.0; /* big enough to transform the distance... */
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			m[i*cols + j] = vstep * (double)gray[i*step + j];
		}
	}

	image_dt(m, rows, cols, depth);
	sqrt_m(m, rows * cols);
	double_to_image(m, rows, cols, gray, step);
	free(m);


	return IMAGE_RET_OK;
}

u8 image_dt_binary(u8 *bimg, int rows, int cols, int step, int depth)
{
	if (bimg == NULL || rows < 1 || cols < 1 || step < 1 || depth != IM_8U)	return IMAGE_RET_ERROR;

	double *m = malloc(sizeof(m[0]) * rows * cols);
	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			m[i*cols + j] = bimg[i*step + j] > 0 ? +INF : 0.0;
		}
	}

	image_dt(m, rows, cols, depth);
	sqrt_m(m, rows*cols);
	double_to_image(m, rows, cols, bimg, step);
	free(m);


	return IMAGE_RET_OK;
}


/*
#include <opencv/cv.h>
#include <opencv/highgui.h>

static void
test_binary(const char *iname) {
		IplImage *gray = cvLoadImage(iname, 0);
		IplImage *binary = NULL;

		if (!gray) {
				return ;
		}

		binary = cvCreateImage(cvGetSize(gray), 8, 1);

		cvThreshold(gray, binary, 50, 255, CV_THRESH_BINARY);

		dt_binary((unsigned char*)binary->imageData, binary->height, binary->width, binary->widthStep);

		cvSaveImage("result-binary.jpg", binary, 0);
		cvNamedWindow("result", 1);
		cvShowImage("result", binary);
		cvWaitKey(0);
		cvDestroyWindow("result");
		cvReleaseImage(&gray);
		cvReleaseImage(&binary);
}

static void
test_gray(const char *iname) {
		IplImage *gray = cvLoadImage(iname, 0);

		if (!gray) {
				return ;
		}

		dt_gray((unsigned char*)gray->imageData, gray->height, gray->width, gray->widthStep);

		cvSaveImage("result-gray.jpg", gray, 0);
		cvNamedWindow("result", 1);
		cvShowImage("result", gray);
		cvWaitKey(0);
		cvDestroyWindow("result");
		cvReleaseImage(&gray);
}

*/