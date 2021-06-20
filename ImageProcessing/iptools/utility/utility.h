#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include <math.h>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, int x, int y, int sx, int sy, int value);
		static void binarize(image &src, image &tgt, int x, int y, int sx, int sy, int threshold);
		static void scale(image &src, image &tgt, float ratio);
		static void thresholded_add(image &src, image &tgt, int x, int y, int sx, int sy, int threshold, int v1, int v2);
		static void doub_thresh(image &src, image &tgt, int x, int y, int sx, int sy, int t1, int t2);
		static void color_mod(image &src, image &tgt, int x, int y, int sx, int sy, int dr, int dg, int db);
		static void smooth_2D(image &src, image &tgt, int x, int y, int sx, int sy, int ws);
		static void smooth_1D(image &src, image &tgt, int x, int y, int sx, int sy, int ws);
		static void histo_stretch(image &src, image &tgt, int x, int y, int sx, int sy, int a, int b, int roi, string hist_name);
		static void make_histo(int arr[], string hist_name);
		static void thresh_stretch(image &src, image &tgt, int x, int y, int sx, int sy, int threshold);
		static void color_stretch(image &src, image &tgt, int x, int y, int sx, int sy, int a, int b);
		static void hsi_stretch(image &src, image &tgt, int x, int y, int sx, int sy, int a, int b);
		static double RGB_to_HSI(double r, double b, double g, int HSI);
		static double * HSI_to_RGB(double h, double s, double i);
		static void edge_detect(image &src, int x, int y, int sx, int sy, int threshold, int direction, int ws, int roi,string name);
		static void hsi_detect(image &src, int x, int y, int sx, int sy, int threshold, int direction, int ws, int roi,string name);
		static void opencv_sobel_grey(cv::Mat &src, int x, int y, int sx, int sy, int threshold, int direction, int ws, int roi,string name);
		static void canny_grey(cv::Mat &src, int x, int y, int sx, int sy, int thresholdmin, int thresholdmax, int ws, int roi,string name);
		static void canny_hsv(cv::Mat &src, int x, int y, int sx, int sy, int thresholdmin, int thresholdmax, int ws, int roi,string name);
		static void HS_CV(cv::Mat &src, int x, int y, int sx, int sy, int roi,string name);
		static void HE_CV(cv::Mat &src, int x, int y, int sx, int sy, int roi,string name);
		static void HEonV(cv::Mat &src, int x, int y, int sx, int sy, int roi,string name);
		static void HEonHSV(cv::Mat &src, int x, int y, int sx, int sy, string hsv, int roi,string name);
		static void HE_Canny(cv::Mat &src, int x, int y, int sx, int sy, int thresholdmin, int thresholdmax, int ws, int roi,string name);
		static void HE_Sobel(cv::Mat &src, int x, int y, int sx, int sy, int threshold, int ws, int roi,string name);
};

#endif

