U80859981 Jacob Hunt

DIPCODE_Linux holds all the source code as well as the project images that are used for testing 
and to demonstrate that the filters/functions work correctly. Images can be found in the "bin" folder.

*************************************************************************************************
iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.

*************************************************************************************************


*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make

As a result you should get iptool in project/bin directory.

*** FUNCTIONS ***

*All work only within regions of interest. This means that each function will accept roi parameters as well as 
filter parameters

1. Add intensity: add
Increase the intensity for a gray-level image.

2. Binarization: binarize
Binarize the pixels with the threshold.

3. Scaling: scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.

4. Modify brightness based on threshold: thresholded_add
Modifies the brigthness if the intensity of the pixel is not equal to the threshold(T)
- Increase the brightness of the pixel by V1 if the pixel is larger than the threshold(T)
- Decrease the the brigtness of the pixel by V2 if the intensity is smaller than the threshold(T)
- Do nothing if the intensity is already equal to the threshold value (T)

5. Double Thresholding color visualization: double_thresh
Pixels with intensity between Threshold 1(T1) and (T2) will be colored red, where T1 <= T2.
- color pixel red if intensity is  T1 <= and >= T2
- do nothing if intensity does not fall withing thresholds

6. Color Brightness Modification: color_mod
Given three RGB values (input-R, input-G, input-B) from a user, function modifies the intensity of each color channel for a pixel
- Process RGB channels like so: R^ = R + input-R, G^ = G + input-G, B^ = B + input-B
- new values must be within acceptable values 0-255


7. Uniform Smoothing: smoothing_2D or smoothing_1D
smooths image based on odd window size(ws), eg. (3,5,7,...)
-2D uniform smoothing
-1D incremental and seperable uniform smoothing
	-dont use edge cases rois for this function

8. Histogram Stretching: histogram_stretch
Given a range [a,b], the intensity historgram of an image is stretched to the range [0,255]
-pixel intensity -> 0 if pixel <= a
-pixel intensity -> 255 if pixel >= b
-else use histogram smoothing formula on pixel
-note that this function also outputs before and after pixel intensity histograms for each roi, called "bhisto_OutputImageName_ROI#.pgm" and "ahisto_OutputImageName_ROI#.pgm" 

9. Histogram Stretching with Threshold: threshold_stretch
Given a threshold value, seperate the image into foreground and background pixels and perfrom histogram stretching on each group seperately
-use a_min and a_max of each group as the [a,b] range to be stretched to [0,255]

10. Histogram stretching RGB values of color image: color_stretch
Given a range [a,b], stretch the intensity histogram of each color channel (Red, Blue, Green) to the range [0,255]

11. Histogram stretching I channel of HSI color space: hsi_stretch
Convert from RGB -> HSI then perform histogram stretching on I channel of HSI color space ([a,b] -> [0,255]), then convert HSI -> RGB to display image.

12. Edge Detection on Grey Level images using sobel: edge_detection
Given input: threshold, direction, and window size.
Uses (3x3) or (5x5) Sobel operater to output gradient amplitude image, thresholded gradient amplitude image, direction image.

13. Edge Detection of Color images using sobel on HSI color space: hsi_edge_detection
Given input: threshold, direction, and window size.
Uses (3x3) or (5x5) Sobel operater to output gradient amplitude image, thresholded gradient amplitude image, direction image.

14. Edge Detection on Grey Level images using sobel and opencv module: sobel_grey
Given input: threshold, direction, and window size.
Uses (3x3) or (5x5) Sobel operater and opencv to output gradient amplitude image, thresholded gradient amplitude image, direction image.

15. Edge Detection on Grey Level images using canny and opencv module: canny_grey
Given input: thresholdmin, thresholdmax, and window size
Uses canny module of opencv to generate a thresholded gradient amplitude image

16. Edge Detection on Color images using canny, opencv module, and HSV color space: canny_hsv
Given input: thresholdmin, thresholdmax, and window size
Uses canny module of opencv to generate a thresholded gradient amplitude image

*****************************************************
 (All functions make use of opencv to perform operations)

17. Histogram Stretching using opencv: HS_CV
Using range of pixels in image ([min,max]), intensity histogram of image is stretched to the range [0,255] 

18.Histogram Equalization using opencv: HE_CV
Equalizes intensity histogram of image

19. Histogram Equalizaion on RGB image using V-Channel: HEonV
Convert RGB image to HSV then perform Histogram Equalization on the V channel

20. Histogram Equalizaion on RGB image using HSV: HEonHSV
Given Input:hsv combination that user wants to use, ex. using h and v channel -> "hv" (make sure to use lowercase)
Convert RGB image to HSV then perform Histogram Equalization using any combination of HSV channels

21. Histogram Equaliztion and Sobel Edge Detection: HE_Sobel
Given input: threshold and window size
Apply Histogram Equalization before Sobel edge detector

22. Histogram Equaliztion and Canny Edge Detection: HE_Canny
Given input: thresholdmin, thresholdmax, and window size
Apply Histogram equaliztion before using Canny edge detector

*** PARAMETERS FILE ***

-Unless otherwise stated, all file formats are .pgm
-There are 2 - 4 lines per image filter
line 1. 
	a. the input file name; 
		-for Color Brightness Modification the input file must be of type .ppm
		-for color image/HSI histogram stretching the output file must be of type .ppm
		-for sobel_grey, canny_grey, and canny_hsv, the input file is of type .jpg
		-for Assignment 4 functions, the input file is of type .jpg
	b. the output file name; 
		-for color image/HSI histogram stretching the output file must be of type .ppm
		-for Color Brightness Modification the output file must be of type.ppm
		-for Double Thresholding color visualization, the output file must be of type .ppm to see colors 
		-for sobel_grey, canny_grey, and canny_hsv, the output file is of type .jpg
		-for Assignment 4 functions, the output file is of type .jpg
	c. the name of the filter. 
		1-8: "add", "binarize", "scale", "thresholded_add", "double_thresh", "color_mod", "smoothing_2D", "smoothing_1D", 
		9-12: "histogram_stretch", "threshold_stretch","color_stretch", "hsi_stretch";
		12-16: "edge_detection","hsi_edge_detection","sobel_grey","canny_grey", "canny_hsv" (Note:make sure to add opencv before filter name for opencv filters)
		17-22: "HS_CV", "HE_CV", "HEonV", "HEonHSV", "HE_Sobel", "HE_Canny" (add opencv before filter names)
	d. number of rois


lines 2 - 4: roi and its parameters
	a. every roi will have a x (top left x location), y (top left y location), SX (size of x) , SY(size of y)
	b. parameters for the filter 

Example of how the parameter file is setup:

	lena.pgm lena_addtest.pgm add 3
	0 0 100 300 100                 //x, y, SX, SY, (filter parameters)intensity add value
	200 80 150 150 50
	400 500 100 200 30
	lena.pgm lena_bitest.pgm binarize 3
	0 0 100 300 100			//x, y, SX, SY, (filter parameters) threshold value
	200 80 150 150 50
	400 500 100 200 30

*** Run the program: ./iptool parameters.txt
-The parameter file can be found in the "bin" folder -> DIPCODE_Linux/project/bin
-It can be edited to run whatever photos or functions you'd like.  
