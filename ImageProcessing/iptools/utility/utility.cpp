#define _USE_MATH_DEFINES
#include "utility.h"
#include <string>
#include <cstring>
#include <vector> 
#include <utility>
#include <cmath>

#define MAXRGB 255
#define MINRGB 0

using namespace cv;

enum hsi {HUE, SAT, INTES};
int xgrad_three[3][3] = {{-1, 0, 1,},{-2, 0, 2,}, {-1, 0, 1}};
int ygrad_three[3][3] = {{-1, -2, -1,}, {0, 0, 0,}, {1, 2, 1}};  
int xgrad_five[5][5] = {{-5,-4,0,4,5},{-8,-10,0,10,8},{-10,-20,0,20,10}
						,{-8,-10,0,10,8},{-5,-4,0,4,5}};
int ygrad_five[5][5] = {{-5,-8,-10,-8,-5},{-4,-10,-20,-10,-4},{0,0,0,0,0}
						,{4,10,20,10,4},{-5,-4,0,4,5}};

image amp,edge,degree;
Mat amplitude,b_amplitude,d_amplitude;
Mat histogram_mod;


std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int x, int y, int sx, int sy, int value)
{
	tgt.copyImage(src);
	//tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++)
		for (int j = x; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{
			tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value)); 
		}
	src.copyImage(tgt);
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int x, int y, int sx, int sy, int threshold)
{
	tgt.copyImage(src);
	//tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++)
	{
		for (int j = x; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}
	}
	src.copyImage(tgt);
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			/* Map the pixel of new image back to original image */
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				/* Directly copy the value */
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				/* Average the values of four pixels */
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::thresholded_add(image &src, image &tgt, int x, int y, int sx, int sy, int threshold, int v1, int v2)
{
	tgt.copyImage(src);
	//tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++)
	{
		for (int j = x; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{
			if (src.getPixel(i,j) > threshold){
		
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+v1)); 
			}
			else if (src.getPixel(i,j) < threshold){
				
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j)-v2)); 
			}
			else
				tgt.setPixel(i,j,src.getPixel(i,j));		
		}
	}

	src.copyImage(tgt);
}

/*-----------------------------------------------------------------------**/
void utility::doub_thresh(image &src, image &tgt, int x, int y, int sx, int sy, int t1, int t2)
{
	tgt.copyImage(src);
	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++)
	{
		for (int j = x; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{
			if(src.getPixel(i,j) >= t1 && src.getPixel(i,j) <= t2)
			{
				tgt.setPixel(i,j,RED,MAXRGB);
				tgt.setPixel(i,j,GREEN,MINRGB);
				tgt.setPixel(i,j,BLUE,MINRGB);
			}	
		}
	}
	src.copyImage(tgt);
}

/*-----------------------------------------------------------------------**/
void utility::color_mod(image &src, image &tgt, int x, int y, int sx, int sy, int dr, int dg, int db)
{
	tgt.copyImage(src);
	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++)
	{
		for (int j = x; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{
				tgt.setPixel(i,j,RED,checkValue(src.getPixel(i,j,RED) + dr));
				tgt.setPixel(i,j,GREEN,checkValue(src.getPixel(i,j,GREEN) + dg));
				tgt.setPixel(i,j,BLUE,checkValue(src.getPixel(i,j,BLUE) + db));
		}
	}
	src.copyImage(tgt);
}

/*-----------------------------------------------------------------------**/
void utility::smooth_2D(image &src, image &tgt, int x, int y, int sx, int sy, int ws)
{
	tgt.copyImage(src);
	int pixelvalue;
	int numpixel;
	int sum;
	int m = (ws - 1)/2;

	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++)
	{
		for (int j = x; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{
			sum = 0;
			numpixel = 1;

			for(int g = i - m; g <= i + m ;g++)
			{
				for(int h = j - m; h <= j + m; h++)
				{
					if(src.isInbounds(g,h) && !(g == i && h == j))
					{
						sum += src.getPixel(g,h);
						numpixel += 1;
					}		
				}
			}

			tgt.setPixel(i,j,checkValue(sum/numpixel));

		}
	}
	src.copyImage(tgt);

}

/*-----------------------------------------------------------------------**/

void utility::smooth_1D(image &src, image &tgt, int x, int y, int sx, int sy, int ws)
{
	tgt.copyImage(src);
	int sum = 0;
	int m = (ws - 1)/2;
	int temp;
	

	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++)
	{
		sum = 0;
		for(int h = x - m; h <= x + m; h++)
		{
			sum += src.getPixel(i,h);
		}

		tgt.setPixel(i,x,checkValue(sum/ws));

		for (int j = x + 1; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{
			sum = sum - src.getPixel(i,j - m -1) + src.getPixel(i,j + m);
			tgt.setPixel(i,j,checkValue(sum/ws));
		}

	}

	for (int j = x; j < min(src.getNumberOfColumns(), (x + sx)); j++)
	{
		sum = 0;
		for(int h = y - m; h <= y + m; h++)
		{
			if(h == y)
		  		sum += tgt.getPixel(h,j);
			else
				sum += src.getPixel(h,j);
				
		}

		temp = tgt.getPixel(y,j);
		tgt.setPixel(y,j,checkValue(sum/ws));

		for (int i = y + 1; i < min(src.getNumberOfRows(), (y + sy)); i++)
		{
			sum = sum - src.getPixel(i - m - 1,j) - temp - src.getPixel(i,j) + tgt.getPixel(i,j) + src.getPixel(i+m , j) + src.getPixel(i - 1,j);
			temp = tgt.getPixel(i,j);
			tgt.setPixel(i,j,checkValue(sum/ws));
		}

	}

	src.copyImage(tgt);
				
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::histo_stretch(image &src, image &tgt, int x, int y, int sx, int sy, int a, int b, int roi, string hist_name)
{
	tgt.copyImage(src);

	static int bhisto[256] = {0};
	static int ahisto[256] = {0};
	double pixel;
	string before = hist_name;
	string after = hist_name;

	before += string("_bhist_") + intToString(roi) + ".pgm";
	after += string("_ahist_") + intToString(roi) + ".pgm";
	

	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++) //histogram stretching
	{
		for (int j = x + 1; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{
			bhisto[src.getPixel(i,j)]++;

			if (src.getPixel(i,j) <= a){
				tgt.setPixel(i,j,0);
				ahisto[0]++;
			}	
			else if (src.getPixel(i,j) >= b){
				tgt.setPixel(i,j,255);
				ahisto[255]++;
			}	
			else{
				pixel = (src.getPixel(i,j) - a) * (255.00 / (b-a)) + 0.5;
				tgt.setPixel(i,j,checkValue((int)pixel));
				ahisto[checkValue((int)pixel)]++;
			}
		}
	}

	make_histo(bhisto, before);//make historgram of org image
	make_histo(ahisto, after);//make histogram of new image

	src.copyImage(tgt);
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::make_histo(int arr[], string hist_name)
{
	image histogram(256, 256);
	int max = arr[0];//max # of pixels
	int y;

	for(int i = 0; i < 256; i++)
	{
		if (arr[i] > max)
			max = arr[i];
	}

	for(int j = 0; j < histogram.getNumberOfColumns(); j++)
	{
		y = (arr[j] * 255) / max;
		//y = checkValue(y);

		for (int i = histogram.getNumberOfRows(); i > histogram.getNumberOfRows() - y; i--)
		{
			histogram.setPixel(i,j,255);
		}
	}

	char outfile[hist_name.length()+1];
	strcpy(outfile, hist_name.c_str());

	histogram.save(outfile);
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::thresh_stretch(image &src, image &tgt, int x, int y, int sx, int sy, int threshold)
{
	tgt.copyImage(src);

	int b_min, b_max, f_min, f_max, pixel;
	double value;
	vector< pair<int,int> > background;
	vector< pair<int,int> > foreground;

	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++) //finds and stores background and foreground pixels
	{

		for (int j = x + 1; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{
			pixel = src.getPixel(i,j);

			if(pixel < threshold)
				background.push_back(make_pair(i,j));
			else
				foreground.push_back(make_pair(i,j));
		}
	}

	if(!background.empty())
	{
		b_min = src.getPixel(background[0].first,background[0].second);
		b_max = b_min;
		for(int i = 0; i < background.size(); i++) //find max and min of background
		{
			pixel = src.getPixel(background[i].first,background[i].second);
			if(pixel > b_max)
				b_max = pixel;
			else if(pixel < b_min)
				b_min = pixel;
		}
	

		for(int i = 0; i < background.size(); i++) // change background
		{
			value = (src.getPixel(background[i].first,background[i].second) - b_min) * (255.00 / (b_max-b_min));
			tgt.setPixel(background[i].first,background[i].second,checkValue((int)value));
		}
	}

	if(!foreground.empty())
	{
		f_min = src.getPixel(foreground[0].first,foreground[0].second);
		f_max = f_min;
		for(int i = 0; i < foreground.size(); i++) //find max and min of foreground 
		{
			pixel = src.getPixel(foreground[i].first,foreground[i].second);
			if(pixel > f_max)
				f_max = pixel;
			else if(pixel < f_min)
				f_min = pixel;
		}

		for(int i = 0; i < foreground.size(); i++) // change foreground
		{
			value = (src.getPixel(foreground[i].first,foreground[i].second) - f_min) * (255.00 / (f_max-f_min));
			tgt.setPixel(foreground[i].first,foreground[i].second,checkValue((int)value));
		}
	}


	src.copyImage(tgt);
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::color_stretch(image &src, image &tgt, int x, int y, int sx, int sy, int a, int b)
{
	tgt.copyImage(src);

	double pixel;

	for(int rgb = 0; rgb < 3; rgb++)
	{
		for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++)
		{

			for (int j = x + 1; j < min(src.getNumberOfColumns(), (x + sx)); j++)
			{
				if(src.getPixel(i,j,rgb) <= a)
					tgt.setPixel(i,j,rgb,0);
				else if(src.getPixel(i,j,rgb) >= b)
					tgt.setPixel(i,j,rgb,255);
				else
				{
					pixel = (src.getPixel(i,j,rgb) - a) * (255.00 / (b-a)) + 0.5;
					tgt.setPixel(i,j,rgb,checkValue((int)pixel));
				}
			}
		}
	}

	src.copyImage(tgt);


}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::hsi_stretch(image &src, image &tgt, int x, int y, int sx, int sy, int a, int b)
{
	tgt.copyImage(src);

	double hue, saturation, intensity;
	double red,blue,green;
	double *rgbs;


	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++)
	{
		for (int j = x + 1; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{
			red = static_cast<double>(src.getPixel(i, j, RED));
			blue = static_cast<double>(src.getPixel(i, j, BLUE));
			green = static_cast<double>(src.getPixel(i, j, GREEN));

			hue = RGB_to_HSI(red, blue, green, HUE);
			saturation = RGB_to_HSI(red, blue, green, SAT);
			intensity = RGB_to_HSI(red, blue, green, INTES);

			if (intensity <= a)
				intensity = 0.0;
			else if (intensity >= b)
				intensity = 255.0;
			else
				intensity = (intensity - a) * (255.00 / (b - a));


			rgbs = HSI_to_RGB(hue, saturation, intensity);

			tgt.setPixel(i, j, RED, checkValue(lround(rgbs[0])));
			tgt.setPixel(i, j, BLUE, checkValue(lround(rgbs[2])));
			tgt.setPixel(i,j,GREEN, checkValue(lround(rgbs[1])));

		}
	}

	src.copyImage(tgt);
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

double utility::RGB_to_HSI(double r, double b, double g,  int HSI)
{
	double value;
	double sum = r + b + g;
	r = r / sum;
	b = b / sum;
	g = g / sum;

	switch(HSI){
		case HUE:
			value = 0.5 * ((r-g) + (r-b));
			value = value / (sqrt(pow(r-g,2) + (r-b)*(g-b)));
			value = acos(value);
			if(b > g)
				value = (2 * M_PI) - value;
			value = value * (180 / M_PI);
			break;
		case SAT:
			value = 1 - (3.0 * min(r,min(b,g)));
			value = value * 100.0;
			break;
		case INTES:
			value = sum / (3.0 * 255);
			value = value * 255.0;
			break;
	}		
	return value; 
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

double * utility::HSI_to_RGB(double h, double s, double i)
{ 	
	static double value[3];

	h = h * (M_PI/180);
	s = s/100.0;
	i = i/255.0;
	
	if( h < (2.0*M_PI/3)){
		value[2] = i * (1-s);
		value[0] = i * (1 + ((s*cos(h))/(cos(M_PI/3-h))));
		value[1] = 3 * i - (value[0] + value[2]);
	}
	else if( (2*M_PI/3) <= h && h < (4*M_PI/3)){
		h = h - (2 * M_PI / 3);

		value[0] = i * (1-s);
		value[1] = i * (1 + ((s*cos(h))/(cos(M_PI/3-h))));
		value[2] = 3 * i - (value[0] + value[1]);

	}
	else{
		h = h - (4 * M_PI / 3);
		value[1] = i * (1-s);
		value[2] = i * (1 + ((s*cos(h))/(cos(M_PI/3-h))));
		value[0] = 3 * i - (value[1] + value[2]);

	}

	value[0] = value[0] * 255;
	value[1] = value[1] * 255;
	value[2] = value[2] * 255;
	

	return value;
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::edge_detect(image &src, int x, int y, int sx, int sy, int threshold, int direction, int ws, int roi, string name)
{

	if( roi == 1)
	{
		amp.copyImage(src);
		edge.copyImage(src);
		degree.copyImage(src);
	}

	int pixel, gradient, gx, gy, k;
	double angle;
	int m = ws/2;
	string amplitude = name + "_amplitude.pgm";
	string edges = name + "_edgeDetection.pgm";
	string degrees = name +"_edgeDegrees.pgm";
	
	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++)
	{
		for (int j = x; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{	
			gy = 0;
			gx = 0;
			for(int g = i - m; g <= i + m ;g++)
			{
				for(int h = j - m; h <= j + m; h++)
				{
					if(src.isInbounds(g,h))
					{
						if(ws == 3){
							gx += xgrad_three[g+m-i][h+m-j] * src.getPixel(g,h);
							gy += ygrad_three[g+m-i][h+m-j] * src.getPixel(g,h);
						}
						else{
							gx += xgrad_five[g+m-i][h+m-j] * src.getPixel(g,h);
							gy += ygrad_five[g+m-i][h+m-j] * src.getPixel(g,h);
						}
					}		
				}
			}
			gradient = (int)(sqrt(((double)gx * gx) + ((double)gy * gy)));
			
			
			if(ws == 3)
				gradient = (int)(gradient / 1443.00 * 255);
			else
				gradient = (int)(gradient /26686.00 * 255); 

			amp.setPixel(i,j,checkValue(gradient));
			angle = abs(atan2((double) gy,(double) gx) * 180 / M_PI);

			if (gradient < threshold)
			{
				edge.setPixel(i,j,MINRGB);
				degree.setPixel(i,j,MINRGB);
			}
			else
			{
				edge.setPixel(i,j,MAXRGB);
				if((direction + 10) >= (int)angle && (int)angle >= (direction - 10)) 
					degree.setPixel(i,j,MAXRGB);
				else
					degree.setPixel(i,j,MINRGB);
			}

						
		}
	}


	amp.save(amplitude.c_str());
	edge.save(edges.c_str());
	degree.save(degrees.c_str());
	

}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::hsi_detect(image &src, int x, int y, int sx, int sy, int threshold, int direction, int ws, int roi, string name)
{
	if( roi == 1)
	{
		amp.copyImage(src);
		edge.copyImage(src);
		degree.copyImage(src);
	}

	int pixel, gradient, gx, gy, k;
	double angle;
	double red,blue,green,intensity,hue,saturation;
	int m = ws/2;
	string amplitude = name + "_amplitude.ppm";
	string edges = name + "_edgeDetection.ppm";
	string degrees = name +"_edgeDegrees.ppm";

	for (int i = y; i < min(src.getNumberOfRows(), (y + sy)); i++)
	{
		for (int j = x; j < min(src.getNumberOfColumns(), (x + sx)); j++)
		{	
			gy = 0;
			gx = 0;
			for(int g = i - m; g <= i + m ;g++)
			{
				for(int h = j - m; h <= j + m; h++)
				{
					if(src.isInbounds(g,h))
					{
						red = static_cast<double>(src.getPixel(g, h, RED));
						blue = static_cast<double>(src.getPixel(g, h, BLUE));
						green = static_cast<double>(src.getPixel(g, h, GREEN));

						intensity = RGB_to_HSI(red, blue, green, INTES);
						saturation = RGB_to_HSI(red, blue, green, SAT);
						hue = RGB_to_HSI(red, blue, green, HUE);

						if(ws == 3){
							gx += xgrad_three[g+m-i][h+m-j] * intensity;
							gy += ygrad_three[g+m-i][h+m-j] * intensity;
						}
						else{
							gx += xgrad_five[g+m-i][h+m-j] * intensity;
							gy += ygrad_five[g+m-i][h+m-j] * intensity;
						}
					}		
				}
			}
			gradient = (int)(sqrt(((double)gx * gx) + ((double)gy * gy)));
			if(ws == 3)
				gradient = (int)(gradient / 1443.00 * 255);
			else
				gradient = (int)(gradient /26686.00 * 255); 

			amp.setPixel(i,j,RED,checkValue(gradient));
			amp.setPixel(i,j,BLUE,checkValue(gradient));
			amp.setPixel(i,j,GREEN,checkValue(gradient));
			angle = abs(atan2((double) gy,(double) gx) * 180 / M_PI);

			if (gradient < threshold)
			{
				edge.setPixel(i,j,RED,MINRGB);
				edge.setPixel(i,j,GREEN,MINRGB);
				edge.setPixel(i,j,BLUE,MINRGB);
				degree.setPixel(i,j,RED,MINRGB);
				degree.setPixel(i,j,GREEN,MINRGB);
				degree.setPixel(i,j,BLUE,MINRGB);
			}
			else
			{
				edge.setPixel(i,j,RED,MAXRGB);
				edge.setPixel(i,j,GREEN,MAXRGB);
				edge.setPixel(i,j,BLUE,MAXRGB);
				
				if((direction + 10) >= (int)angle && (int)angle >= (direction - 10)){
					degree.setPixel(i,j,RED,MAXRGB);
					degree.setPixel(i,j,GREEN,MAXRGB);
					degree.setPixel(i,j,BLUE,MAXRGB);
				}
				else{
					degree.setPixel(i,j,RED,MINRGB);
					degree.setPixel(i,j,GREEN,MINRGB);
					degree.setPixel(i,j,BLUE,MINRGB);
				}

			}

						
		}
	}

	amp.save(amplitude.c_str());
	edge.save(edges.c_str());
	degree.save(degrees.c_str());
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::opencv_sobel_grey(Mat &src, int x, int y, int sx, int sy, int threshold, int direction, int ws, int roi,string name)
{
	if(roi == 1)
	{
		cvtColor( src , src , COLOR_BGR2GRAY);
		amplitude = src.clone();
		b_amplitude = src.clone();
		d_amplitude = src.clone();
	}

	Mat x_grad,y_grad,temp;
	Sobel(src,x_grad, CV_16S,1,0,ws);
	Sobel(src,y_grad, CV_16S,0,1,ws);

	if(ws == 3)
	{
		convertScaleAbs(x_grad,x_grad,	 255/1020.00,0);
		convertScaleAbs(y_grad,y_grad,  255/1020.00,0);
	}
	else
	{
		convertScaleAbs(x_grad,x_grad, 255/18870.00,0);
		convertScaleAbs(y_grad,y_grad, 255/18870.00,0);
	}

	unsigned char gradient;
	double angle;

	;

	string amplit = name + "_amplitude.jpg";
	string edges = name + "_edgeDetection.jpg";
	string degrees = name +"_edgeDegrees.jpg";

	for (int i = y; i < min(src.rows, (y + sy)); i++)
	{
		for (int j = x + 1; j < min(src.cols, (x + sx)); j++)
		{	
			angle = abs(atan2((double)y_grad.at<unsigned char>(i,j),(double)x_grad.at<unsigned char>(i,j)) * 180.00 / M_PI);
			
			gradient = sqrt((y_grad.at<unsigned char>(i,j) * y_grad.at<unsigned char>(i,j)) + (x_grad.at<unsigned char>(i,j) * x_grad.at<unsigned char>(i,j)));
		
			amplitude.at<unsigned char>(i,j) = gradient;

			if(gradient < threshold){
				b_amplitude.at<unsigned char>(i,j) = 0;
				d_amplitude.at<unsigned char>(i,j) = 0;
			}
			else{
				b_amplitude.at<unsigned char>(i,j) = 255;
				if((direction + 10) >= (int)angle && (int)angle >= (direction - 10))
					d_amplitude.at<unsigned char>(i,j) = 255;
				else
					d_amplitude.at<unsigned char>(i,j) = 0;
			}			
		}
	}

	imwrite(amplit, amplitude);
	imwrite(edges, b_amplitude);
	imwrite(degrees, d_amplitude);
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::canny_grey(Mat &src, int x, int y, int sx, int sy, int thresholdmin, int thresholdmax, int ws, int roi,string name)
{
	if(roi == 1)
	{
		cvtColor( src , src , COLOR_BGR2GRAY);
		b_amplitude = src.clone();
		GaussianBlur ( src , src , Size(5,5) , 0 , 0 );
	}

	Mat temp;
	Canny(src,temp, thresholdmin, thresholdmax, ws);

	for (int i = y; i < min(src.rows, (y + sy)); i++)
	{
		for (int j = x + 1; j < min(src.cols, (x + sx)); j++)
		{	
			b_amplitude.at<unsigned char>(i,j) = temp.at<unsigned char>(i,j);
		}
	}

	imwrite(name, b_amplitude);
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::canny_hsv(Mat &src, int x, int y, int sx, int sy, int thresholdmin, int thresholdmax, int ws, int roi,string name)
{
	if(roi == 1)
	{
		b_amplitude = src.clone();
		cvtColor( src , src , COLOR_BGR2HSV);
		GaussianBlur ( src , src , Size(5,5) , 0 , 0 );
	}

	Mat hsv[3], vChannel;
	split(src,hsv);

	Canny(hsv[2], vChannel, thresholdmin, thresholdmax, ws);

	for (int i = y; i < min(src.rows, (y + sy)); i++)
	{
		for (int j = x + 1; j < min(src.cols, (x + sx)); j++)
		{	

			b_amplitude.data[b_amplitude.channels()*(b_amplitude.cols*i + j) + 0] = vChannel.at<unsigned char>(i,j);
			b_amplitude.data[b_amplitude.channels()*(b_amplitude.cols*i + j) + 1] = vChannel.at<unsigned char>(i,j);
			b_amplitude.data[b_amplitude.channels()*(b_amplitude.cols*i + j) + 2] = vChannel.at<unsigned char>(i,j);
		}
	}		

	imwrite(name, b_amplitude);
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::HS_CV(cv::Mat &src, int x, int y, int sx, int sy, int roi,string name)
{
	if(roi == 1)
	{
		cvtColor( src , src , COLOR_BGR2GRAY);
		histogram_mod = src.clone();	

		normalize(histogram_mod, histogram_mod, 0, 255, NORM_MINMAX);
	}

	for (int i = y; i < min(src.rows, (y + sy)); i++)
		for (int j = x + 1; j < min(src.cols, (x + sx)); j++)
			src.at<unsigned char>(i,j) = histogram_mod.at<unsigned char>(i,j);

		
	imwrite(name, src);

}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::HE_CV(cv::Mat &src, int x, int y, int sx, int sy, int roi,string name)
{
	if(roi == 1)
	{
		cvtColor( src , src , COLOR_BGR2GRAY);
		histogram_mod = src.clone();	

		equalizeHist(histogram_mod, histogram_mod);
	}

	for (int i = y; i < min(src.rows, (y + sy)); i++)
		for (int j = x + 1; j < min(src.cols, (x + sx)); j++)
			src.at<unsigned char>(i,j) = histogram_mod.at<unsigned char>(i,j);

	imwrite(name, src);		
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::HEonV(cv::Mat &src, int x, int y, int sx, int sy, int roi,string name)
{
	if(roi == 1)
	{
		Mat temp = src.clone();
		cvtColor(temp, temp, COLOR_BGR2HSV);

		vector<Mat> hsvChannels(3);
		split(temp, hsvChannels);

		equalizeHist(hsvChannels[2],hsvChannels[2]);

		merge(hsvChannels, histogram_mod);

		cvtColor(histogram_mod, histogram_mod, COLOR_HSV2BGR);

	}

	for (int i = y; i < min(src.rows, (y + sy)); i++)
		for (int j = x; j < min(src.cols, (x + sx)); j++)
			src.at<Vec3b>(i,j) = histogram_mod.at<Vec3b>(i,j);

	imwrite(name, src);	
	
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::HEonHSV(cv::Mat &src, int x, int y, int sx, int sy, string hsv, int roi,string name)
{
	if(roi == 1)
	{
		histogram_mod = src.clone();
		cvtColor(histogram_mod, histogram_mod, COLOR_BGR2HSV);
	}

	Mat temp;
	vector<Mat> hsvC(3);

	split(histogram_mod, hsvC);

	if(hsv.find("h") != string::npos)
		equalizeHist(hsvC[0], hsvC[0]);
	if(hsv.find("s") != string::npos)
		equalizeHist(hsvC[1], hsvC[1]);
	if(hsv.find("v") != string::npos)
		equalizeHist(hsvC[2], hsvC[2]);

	merge(hsvC,temp);

	cvtColor(temp, temp, COLOR_HSV2BGR);

	for (int i = y; i < min(src.rows, (y + sy)); i++)
		for (int j = x + 1; j < min(src.cols, (x + sx)); j++)
			src.at<Vec3b>(i,j) = temp.at<Vec3b>(i,j);

	imwrite(name, src);	
	
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::HE_Canny(Mat &src, int x, int y, int sx, int sy, int thresholdmin, int thresholdmax, int ws, int roi,string name)
{
	if(roi == 1)
	{
		cvtColor( src , src , COLOR_BGR2GRAY);

		b_amplitude = src.clone();
		equalizeHist(b_amplitude, b_amplitude);

		amplitude = src.clone();

		GaussianBlur ( amplitude , amplitude , Size(3,3) , 0 , 0 );
		GaussianBlur ( b_amplitude , b_amplitude , Size(3,3) , 0 , 0 );

		d_amplitude = src.clone();
	}

	Mat temp, temp2;

	Canny(amplitude,temp, thresholdmin, thresholdmax, ws); //no HE
	Canny(b_amplitude, temp2, thresholdmin, thresholdmax, ws); //HE before

	for (int i = y; i < min(src.rows, (y + sy)); i++)
	{
		for (int j = x; j < min(src.cols, (x + sx)); j++)
		{
			src.at<unsigned char>(i,j) = temp2.at<unsigned char>(i,j);
			d_amplitude.at<unsigned char>(i,j) = temp2.at<unsigned char>(i,j) - temp.at<unsigned char>(i,j);
		}
	}

	imwrite(name, src);	
	imwrite(name + "_subtraction.jpg", d_amplitude);	
}

/*----------------------------------------------------------------------------------------------------------------------------------**/

void utility::HE_Sobel(Mat &src, int x, int y, int sx, int sy, int threshold, int ws, int roi,string name)
{
	if(roi == 1)
	{
		cvtColor( src , src , COLOR_BGR2GRAY);

		b_amplitude = src.clone();
		equalizeHist(b_amplitude, b_amplitude);

		amplitude = src.clone();
		d_amplitude = src.clone();
		
	}

	Mat x_grad, y_grad, x_gradHE, y_gradHE,temp;
	temp = d_amplitude.clone();
	unsigned char gradient, gradientHE; 

	/*
	//no HE
	Sobel(d_amplitude,x_grad, CV_16S,1,0,ws);
	Sobel(d_amplitude,y_grad, CV_16S,0,1,ws);

	//HE
	Sobel(b_amplitude,x_gradHE, CV_16S,1,0,ws);
	Sobel(b_amplitude,y_gradHE, CV_16S,0,1,ws);

	if(ws == 3)
	{
		convertScaleAbs(x_grad,x_grad);
		convertScaleAbs(y_grad,y_grad);

		convertScaleAbs(x_gradHE,x_gradHE);
		convertScaleAbs(y_gradHE,y_gradHE);
	}
	else
	{
		convertScaleAbs(x_grad,x_grad, 255/18870.00,0);
		convertScaleAbs(y_grad,y_grad, 255/18870.00,0);

		convertScaleAbs(x_gradHE,x_gradHE, 255/18870.00,0);
		convertScaleAbs(y_gradHE,y_gradHE, 255/18870.00,0);
	}
	*/

	Sobel(d_amplitude,x_grad, -1,1,1,ws);
	Sobel(b_amplitude,x_gradHE, -1,1,1,ws);
	
	for (int i = y; i < min(src.rows, (y + sy)); i++)
	{
		for (int j = x + 1; j < min(src.cols, (x + sx)); j++)
		{	
			/*
			gradient = sqrt((y_grad.at<unsigned char>(i,j) * y_grad.at<unsigned char>(i,j)) + (x_grad.at<unsigned char>(i,j) * x_grad.at<unsigned char>(i,j)));
			gradientHE = gradient = sqrt((y_gradHE.at<unsigned char>(i,j) * y_gradHE.at<unsigned char>(i,j)) + (x_gradHE.at<unsigned char>(i,j) * x_gradHE.at<unsigned char>(i,j)));
			*/
			
			gradient = x_grad.at<unsigned char>(i,j);
			gradientHE = x_gradHE.at<unsigned char>(i,j);
			

			if(gradientHE < threshold)
				src.at<unsigned char>(i,j) = 0;
			else 
				src.at<unsigned char>(i,j) = 255;

			if(gradient < threshold)
				temp.at<unsigned char>(i,j) = 0;
			else
				temp.at<unsigned char>(i,j) = 255;

			amplitude.at<unsigned char>(i,j) = src.at<unsigned char>(i,j) - temp.at<unsigned char>(i,j);
		}
	}

	imwrite(name, src);
	imwrite(name + "_subtraction.jpg", amplitude);
}