#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//Set treshold values
Scalar redLow = Scalar(0, 175, 204);
Scalar redHigh = Scalar(10, 242, 255);
Scalar greenLow = Scalar(85, 110, 140);
Scalar greenHigh = Scalar(95, 244, 357);
Scalar orangeLow = Scalar(12, 137, 206);
Scalar orangeHigh = Scalar(30, 255, 255);


int main() {
	Mat img = imread("light.png");  //Load original image
	imshow("Original image", img);  //Show original image
	
	Mat copy; Mat copy2; Mat red_con; Mat green_con; Mat orange_con; //Set variables
	img.copyTo(copy); //Make a copy

	cvtColor(img, img, COLOR_BGR2HSV); //Convert image to HSV

	Mat mask_red; Mat mask_green; Mat mask_orange; Mat circless; //Set variables to treshold
	inRange(img, redLow, redHigh, mask_red); //Red threshold
	inRange(img, greenLow, greenHigh, mask_green); //Green threshold
	inRange(img, orangeLow, orangeHigh, mask_orange); //Orange threshold

	vector<vector<Point>> contours_red; vector<vector<Point>> contours_green; vector<vector<Point>> contours_orange; //Set vectors for contours
	findContours(mask_red, contours_red, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //Find red contours
	findContours(mask_green, contours_green, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //Find green contours
	findContours(mask_orange, contours_orange, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //Find orange contours

	//Set vectors and store variables into them
	vector<vector<Point> > contours_red_poly(contours_red.size()); vector<vector<Point> > contours_green_poly(contours_green.size()); vector<vector<Point> > contours_orange_poly(contours_orange.size());
	vector<Point2f>centers_red(contours_red.size()); vector<Point2f>centers_green(contours_green.size()); vector<Point2f>centers_orange(contours_orange.size());
	vector<float>radius_red(contours_red.size()); vector<float>radius_green(contours_green.size()); vector<float>radius_orange(contours_orange.size());

	for (size_t i = 0; i < contours_red.size(); i++)  //Iterate through red contours
	{
		approxPolyDP(contours_red[i], contours_red_poly[i], 3, true); //approximates each contour to polygon
		minEnclosingCircle(contours_red_poly[i], centers_red[i], radius_red[i]); //Define min and max distance between circles
	}
	for (size_t i = 0; i < contours_red.size(); i++) // Iterate through the red contours
	{
		drawContours(red_con, contours_red, i, Scalar(0, 255, 255), 3);  //draw red contours
		if (contourArea(contours_red[i]) > 50) // Check if contour is bigger than 50
		{
			circle(copy, centers_red[i], (int)radius_red[i], Scalar(0, 255, 255), 2); //Draw circles around the colors
		}

	}

	//Very simular proccess as above, but for green
	for (size_t i = 0; i < contours_green.size(); i++)
	{
		approxPolyDP(contours_green[i], contours_green_poly[i], 3, true);
		minEnclosingCircle(contours_green_poly[i], centers_green[i], radius_green[i]);
	}
	for (size_t i = 0; i < contours_green.size(); i++)
	{
		drawContours(green_con, contours_green, i, Scalar(0, 255, 255), 3);
		if (contourArea(contours_green[i]) > 50)
		{
			circle(copy, centers_green[i], (int)radius_green[i], Scalar(0, 0, 255), 2);
		}
	}

	//Very simular proccess as above, but for orange
	for (size_t i = 0; i < contours_orange.size(); i++)
	{
		approxPolyDP(contours_orange[i], contours_orange_poly[i], 3, true);
		minEnclosingCircle(contours_orange_poly[i], centers_orange[i], radius_orange[i]);
	}
	for (size_t i = 0; i < contours_orange.size(); i++)
	{
		drawContours(orange_con, contours_orange, i, Scalar(0, 255, 255), 3);
		if (contourArea(contours_orange[i]) > 30 && contourArea(contours_orange[i]) < 100) // check if pixel is bigger than 30 and lower than 100
		{
			circle(copy, centers_orange[i], (int)radius_orange[i], Scalar(255, 0, 0), 2);
		}
	}

	imshow("final.jpg", copy); //Show final image
	waitKey(0);
	return 0;
}