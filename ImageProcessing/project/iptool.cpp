/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threhold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "../iptools/core.h"
#include <strings.h>
#include <string.h>
#include<stdio.h> 

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char infile[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while(fgets(str,MAXLEN,fp) != NULL) {

		pch = strtok(str, " ");
		strcpy(infile, pch);

		pch = strtok(NULL, " ");
		strcpy(outfile, pch);
		string hist_name(pch);

		pch = strtok(NULL, " ");

		if(strncmp(pch,"opencv",6)==0)
		{
			pch = strtok(NULL, " ");

			cv::Mat I = cv::imread(infile);
			//cv::Mat I2;
			
			if( I.empty()) {
				cout << "Could not open or find the image: " << infile << endl;
				return -1;
			}
			

			if (strncmp(pch,"sobel_grey",10)==0)
			{
				pch = strtok(NULL, " ");

				for(int i = 0; i < 4 ; i++)// gets rid of .pgm
					hist_name.resize(hist_name.size() - 1);

				char *params[7];	

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					params[6] = strtok(NULL, " ");
					utility::opencv_sobel_grey(I,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]),atoi(params[6]), i+1 , hist_name);
				}
			} 

			else if (strncmp(pch,"canny_grey",10)==0)
			{
				pch = strtok(NULL, " ");

				char *params[7];	

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					params[6] = strtok(NULL, " ");
					utility::canny_grey(I,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]),atoi(params[6]), i+1 , hist_name);
				}
			}

			else if (strncmp(pch,"canny_hsv",9)==0)
			{
				pch = strtok(NULL, " ");

				char *params[7];	

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					params[6] = strtok(NULL, " ");
					utility::canny_hsv(I,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]),atoi(params[6]), i+1 , hist_name);
				}
			}  

			else if (strncmp(pch,"HS_opencv",9)==0)
			{
				pch = strtok(NULL, " ");

				char *params[4];	

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					utility::HS_CV(I,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]), i+1, hist_name);
				}
			}

			else if (strncmp(pch,"HE_opencv",9)==0)
			{
				pch = strtok(NULL, " ");

				char *params[4];	

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					utility::HE_CV(I,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]), i+1, hist_name);
				}
			}

			else if (strncmp(pch,"HE_V",4)==0)
			{
				pch = strtok(NULL, " ");

				char *params[4];	

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					utility::HEonV(I,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),i+1, hist_name);
				}
			}

			else if (strncmp(pch,"HE_HSV",6)==0)
			{
				pch = strtok(NULL, " ");

				char *params[5];	

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					utility::HEonHSV(I,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),string(params[4]),i+1, hist_name);
				}
			}

			
			else if (strncmp(pch,"HE_Canny",8)==0)
			{
				pch = strtok(NULL, " ");

				char *params[7];	

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					params[6] = strtok(NULL, " ");
					utility::HE_Canny(I,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]),atoi(params[6]),i+1, hist_name);
				}
			}

			else if (strncmp(pch,"HE_Sobel",8)==0)
			{
				pch = strtok(NULL, " ");

				char *params[6];	

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					utility::HE_Sobel(I,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]),i+1, hist_name);
				}
			}

			else {
				printf("No function: %s\n", pch);
				continue;
			}

			//cv::imwrite(outfile, I2);
		}
		else{
			src.read(infile);

			if (strncasecmp(pch,"add",MAXLEN)==0) {
				/* Add Intensity */
				pch = strtok(NULL, " "); //# of rois
				char *params[5];
				

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					utility::addGrey(src,tgt,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]));
				}


			}

			else if (strncasecmp(pch,"binarize",MAXLEN)==0) {
				/* Thresholding */
				pch = strtok(NULL, " "); //# of rois
				char *params[5];

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					utility::binarize(src,tgt,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]));
				}

			}

			else if (strncasecmp(pch,"scale",MAXLEN)==0) {
				/* Image scaling */
				pch = strtok(NULL, " ");
				utility::scale(src,tgt,atof(pch));
			}

			else if (strncasecmp(pch,"thresholded_add",MAXLEN)==0) {
				pch = strtok(NULL, " ");// # of rois
				char *params[7];

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					params[6] = strtok(NULL, " ");
					utility::thresholded_add(src,tgt,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]),atoi(params[6]));
				}
			}

			else if (strncasecmp(pch,"double_thresh",MAXLEN)==0) {
				pch = strtok(NULL, " "); //# of rois
				char *params[6];


				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					utility::doub_thresh(src,tgt,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]));
				}
			}	

			else if (strncasecmp(pch,"color_mod",MAXLEN)==0) 
			{
				pch = strtok(NULL, " "); //# of rois
				char *params[7];

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					params[6] = strtok(NULL, " ");
					utility::color_mod(src,tgt,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]),atoi(params[6]));
				}
			}

			else if (strncasecmp(pch,"smoothing_2D",MAXLEN)==0)
			{
				pch = strtok(NULL, " "); //# of rois
				char *params[5];

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					utility::smooth_2D(src,tgt,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]));
				}

			}

			else if (strncasecmp(pch,"smoothing_1D",MAXLEN)==0)
			{
				pch = strtok(NULL, " "); //# of rois
				char *params[5];

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					utility::smooth_1D(src,tgt,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]));
				}
			}

			else if (strncasecmp(pch,"histogram_stretch",MAXLEN)==0)
			{
				pch = strtok(NULL, " "); //# of roi

				for(int i = 0; i < 4 ; i++)// gets rid of .pgm
					hist_name.resize(hist_name.size() - 1);

				char *params[6];

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					utility::histo_stretch(src,tgt,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]), i+1, hist_name);
				}
			}

			else if (strncasecmp(pch,"threshold_stretch",MAXLEN)==0)
			{
				pch = strtok(NULL, " "); //# of rois
				char *params[5];

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					utility::thresh_stretch(src,tgt,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]));
				}
			}

			else if (strncasecmp(pch,"color_stretch",MAXLEN)==0)
			{
				pch = strtok(NULL, " "); //# of roi
				char *params[6];

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					utility::color_stretch(src,tgt,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]));
				}
			}

			else if (strncasecmp(pch,"hsi_stretch",MAXLEN)==0)
			{
				pch = strtok(NULL, " "); //# of roi
				char *params[6];

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					utility::hsi_stretch(src,tgt,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]));
				}
			}

			else if (strncasecmp(pch,"edge_detection",MAXLEN)==0)
			{
				pch = strtok(NULL, " ");

				for(int i = 0; i < 4 ; i++)// gets rid of .pgm
					hist_name.resize(hist_name.size() - 1);

				char *params[7];	

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					params[6] = strtok(NULL, " ");
					utility::edge_detect(src,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]),atoi(params[6]), i+1 , hist_name);
				}
			}

			else if (strncasecmp(pch,"hsi_edge_detection",MAXLEN)==0)
			{
				
				pch = strtok(NULL, " ");

				for(int i = 0; i < 4 ; i++)// gets rid of .pgm
					hist_name.resize(hist_name.size() - 1);

				char *params[7];	

				for(int i = 0; i < atoi(pch); i++){
					fgets(str,MAXLEN,fp);
					params[0] = strtok(str, " ");
					params[1] = strtok(NULL, " ");
					params[2] = strtok(NULL, " ");
					params[3] = strtok(NULL, " ");
					params[4] = strtok(NULL, " ");
					params[5] = strtok(NULL, " ");
					params[6] = strtok(NULL, " ");
					utility::hsi_detect(src,atoi(params[0]),atoi(params[1]),atoi(params[2]),atoi(params[3]),atoi(params[4]),atoi(params[5]),atoi(params[6]), i+1 , hist_name);
				}
			}

			

			else {
				printf("No function: %s\n", pch);
				continue;
			}
		
			tgt.save(outfile);
		}
	}
	fclose(fp);
	return 0;
}

