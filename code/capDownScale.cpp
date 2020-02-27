
//---------------------------------------------------------------------------
// Integer based image downscaling with subpixel accuracy
// Siniša Petriæ
//---------------------------------------------------------------------------
#include "pch.h"
#include <iostream>
#include "opencv2/opencv.hpp"
using namespace cv;
// color space scaling
const unsigned long spe_SP_NORM           = 4194304;     // 2 ^ 22
const unsigned long spe_SP_SHIFT          = 22;
typedef unsigned char Byte;
//---------------------------------------------------------------------------
void DownsampleRGB(Mat &imgIn, Mat &imgOut)
{
//--------------------------------------------------------------------
// Algorithm: https://sigmapi-design.com/media/fast_box_filter.pdf
// Original code uses different image container.
// I have modified the code to suite opencv Mat, but did not make any further optimization.
//--------------------------------------------------------------------
// imgOut->Zero(); output to zero
unsigned long norm = spe_SP_NORM;
unsigned long shift = spe_SP_SHIFT;
unsigned int x, y, yi, xi, xoc, yi_start, xi_start, yi_end, xi_end, xo_start, xo_end, yo_start, yo_end;
// complete inp/out image
// for inp/out ROIs change code to suite your needs
Rect iBound = Rect(0, 0, imgIn.cols, imgIn.rows);
Rect oBound = Rect(0, 0, imgOut.cols, imgOut.rows);
yi_start = iBound.y;
xi_start = iBound.x;
yi_end   = iBound.height;
xi_end   = iBound.width;
yo_start = oBound.y;
xo_start = oBound.x;
yo_end   = oBound.height;
xo_end   = oBound.width;

float fX = (float)iBound.width / (float)oBound.width;
float fY = (float)iBound.height / (float)oBound.height;
unsigned long pixContribX = (unsigned long)((float)norm / fX);
unsigned long pixContribY = (unsigned long)((float)norm / fY);
unsigned long sumContribX, sumContribY, restContribY, yContrib;
unsigned long outVal[3];
unsigned long *cumulY[3];
Vec3b* inPixel, *outPixel;
for (int i = 0; i < 3; i++)
	{
	cumulY[i] = new unsigned long [oBound.width];
	memset(cumulY[i], 0, oBound.width * sizeof(unsigned long));
	}
bool outX, outY, outLastRow;
y = yo_start;
sumContribY = norm - pixContribY;
restContribY = 0;
yContrib = pixContribY;
outY = false;
outLastRow = false;
outPixel = imgOut.ptr<Vec3b>(y); // point to first pixel in row (output)
for (yi = yi_start; yi < yi_end; yi++)
	{
	x = xo_start;
	xoc = 0;
	outVal[0] = outVal[1] = outVal[2] = 0;
	sumContribX = norm;
	inPixel = imgIn.ptr<Vec3b>(yi); // point to first pixel in row (input)
	for (xi = xi_start; xi < xi_end; xi++)
		{
		// first pass -> X
		outX = false;
		if (sumContribX > pixContribX)
		   {
		   outVal[0] += (Byte)inPixel[xi][0] * pixContribX;
		   outVal[1] += (Byte)inPixel[xi][1] * pixContribX;
		   outVal[2] += (Byte)inPixel[xi][2] * pixContribX;
		   sumContribX -= pixContribX;
		   continue;
		   }
		// rest of contribution factor (sumContrib < pixContrib)
		outVal[0] += (unsigned long)inPixel[xi][0] * sumContribX;
		outVal[1] += (unsigned long)inPixel[xi][1] * sumContribX;
		outVal[2] += (unsigned long)inPixel[xi][2] * sumContribX;
		// done - output pixel to y culmulative
		cumulY[0][xoc] += (outVal[0]>>shift) * yContrib;
		cumulY[1][xoc] += (outVal[1]>>shift) * yContrib;
		cumulY[2][xoc] += (outVal[2]>>shift) * yContrib;
		if (outY)
		   {
		   // done - output pixel
		   outPixel[x][0] = (Byte)(cumulY[0][xoc] >>shift);
		   outPixel[x][1] = (Byte)(cumulY[1][xoc] >>shift);
		   outPixel[x][2] = (Byte)(cumulY[2][xoc] >>shift);
		   // contribution factor for next pixels block (pixContribY - sumContribY)
		   cumulY[0][xoc] = (outVal[0]>>shift) * restContribY;
		   cumulY[1][xoc] = (outVal[1]>>shift) * restContribY;
		   cumulY[2][xoc] = (outVal[2]>>shift) * restContribY;
		   }
		// contribution factor for next pixels block (pixContribX - sumContribX)
		sumContribX = pixContribX - sumContribX;
		outVal[0] = (unsigned long)inPixel[xi][0] * sumContribX;
		outVal[1] = (unsigned long)inPixel[xi][1] * sumContribX;
		outVal[2] = (unsigned long)inPixel[xi][2] * sumContribX;
		sumContribX = norm - sumContribX;
		//
		outX = true;
		x++;
		xoc = x - xo_start;
		if (x >= xo_end)
		   break;
		}
	// output last pixel
	if (!outX)
	   {
	   cumulY[0][xoc] += (outVal[0]>>shift)*yContrib;
	   cumulY[1][xoc] += (outVal[1]>>shift)*yContrib;
	   cumulY[2][xoc] += (outVal[2]>>shift)*yContrib;
	   if (outY)
		  {
		  //outPixel = imgOut.ptr<Vec3b>(y); // point to first pixel in row (output)
		  // done - output pixel
		  outPixel[x][0] = (Byte)(cumulY[0][xoc] >>shift);
		  outPixel[x][1] = (Byte)(cumulY[1][xoc] >>shift);
		  outPixel[x][2] = (Byte)(cumulY[2][xoc] >>shift);
		  // contribution factor for next pixels block (pixContribY - sumContribY)
		  cumulY[0][xoc] = (outVal[0]>>shift) * restContribY;
		  cumulY[1][xoc] = (outVal[1]>>shift) * restContribY;
		  cumulY[2][xoc] = (outVal[2]>>shift) * restContribY;
		  }
	   }
	if (outY)
	   {
	   if (y == yo_end-1)
		  {
		  outLastRow = true;
		  break;
		  }
	   y++;
	   outPixel = imgOut.ptr<Vec3b>(y); // point to first pixel in row (output)
	   }
	if (y >= yo_end)
	   break;
	outY = false;
	if (sumContribY > pixContribY)
	   {
	   yContrib = pixContribY;
	   sumContribY -= pixContribY;
	   continue;
	   }
	yContrib = sumContribY;
	restContribY = pixContribY - sumContribY;
	sumContribY = norm - restContribY;
	outY = true;
	}
// output last row
if (!outLastRow)
   {
   for (unsigned int xo = xo_start; xo < xo_end; xo++)
	   {
	   outPixel[xo][0] = (Byte)(cumulY[0][xo - xo_start] >>shift);
	   outPixel[xo][1] = (Byte)(cumulY[1][xo - xo_start] >>shift);
	   outPixel[xo][2] = (Byte)(cumulY[2][xo - xo_start] >>shift);
	   }
   }
for (int i = 0; i < 3; i++)
	delete []cumulY[i];
}
//---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  argc--;
  argv++;
  if (argc != 2) { fprintf(stderr, " Bad arguments! Calling sequence:\n"
								   " capDownScale <input_image> <downscale_by> (soi)\n"
                                   " <input_image> -> path to input image\n"
                                   " <downscale_by> -> denominator in downscale factor (1/downscale_by)\n"
                                   " (soi) -> show src image: y|n. optional argument, default = n"); exit(1); }
	// String inputFilename("D:/art/shalenkin/photo-226659.jpg"); //test
	String inputFilename = argv[0];
	int divisor = atoi(argv[1]);
	bool showSrc = false;
	if (argc > 2)
		showSrc = (argv[2] == "y");
	Mat srcImage = imread(inputFilename, IMREAD_COLOR);
	/* resize src image window if image is too large
	namedWindow("Source image", WINDOW_NORMAL);
	resizeWindow("Source image", 600,600)
	*/
	std::cout << "Source image W x H =  " << srcImage.cols << " x " << srcImage.rows << std::endl;
	if (showSrc)
		{
		imshow("Source image", srcImage);
		waitKey(2); // Wait for a keystroke in the window
		}
	Mat outImage = Mat::zeros(srcImage.size()/divisor, srcImage.type()); 
	std::cout << "Downsampling by " << divisor << "..." << std::endl;
	clock_t tStart = clock();
	DownsampleRGB(srcImage, outImage);
	std::cout << "Done. Duration: "<<(double)(clock() - tStart) / CLOCKS_PER_SEC << std::endl;
	imshow("Output image", outImage);
	waitKey(0); // Wait for a keystroke in the window
	return 0;
}

