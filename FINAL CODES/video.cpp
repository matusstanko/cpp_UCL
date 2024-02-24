#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main13132() {
    VideoCapture cap("car4.mp4");  //try to open video
    if (!cap.isOpened()) {    //Check if successful
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    while (1) {  //While loop
		
        Mat frame;  //Define frame
        cap >> frame;  //capture frame

        if (frame.empty())  //if frame is empty, start again
            break;

		rotate(frame, frame, ROTATE_180);  //Rotate frame 180 degrees

		Scalar PlateLow = Scalar(150, 1, 170);  //Low threshold points
		Scalar PlateHigh = Scalar(180, 60, 235); //High threshold points
		Mat img = frame;    //Save frame into img
		int cannyn = 250;  //Set canny low point to 250
		Mat out; Mat plates; Mat out2;  //Set variables
		img.copyTo(out);  // Copy img to out
		img.copyTo(out2); // Copy img to out2

		cvtColor(img, img, COLOR_BGR2HSV);  //Convert image to HSV

		inRange(img, PlateLow, PlateHigh, plates); //Do thresholding
		
		Canny(plates, plates, cannyn, (cannyn * 3)); //Do canny edge detection
		
		Mat kernel = getStructuringElement(MORPH_RECT, Size(4, 4)); //Dilate
		dilate(plates, plates, kernel);
		
		vector < vector < Point>> contours; //Set vector for contours
		findContours(plates, contours, RETR_LIST, CHAIN_APPROX_SIMPLE); //Find contours

		//Set vectors
		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
		vector<Point2f>centers(contours.size());
		vector<float>radius(contours.size());

		for (size_t i = 0; i < contours.size(); i++) //Iterate through contours
		{
			approxPolyDP(contours[i], contours_poly[i], 3, true); //approximates each contour to polygon
			boundRect[i] = boundingRect(contours_poly[i]); 
			minEnclosingCircle(contours_poly[i], centers[i], radius[i]);  //define min and max distance values 
		}
		Mat drawing = Mat::zeros(plates.size(), CV_8UC3);
		
		for (size_t i = 0; i < contours.size(); i++) // Iterate through contours
		{
			if (contourArea(contours[i]) > 1700)  //If contour area is larger than 1700
			{
				//If rect width > 150 and < 215,  and height is > 60 and < 160
				if (boundRect[i].width > 150 && boundRect[i].width < 215 && boundRect[i].height > 60 && boundRect[i].height < 160) 
				{
					Mat ROI; //Set ROI variable
					ROI = out2(boundRect[i]);  //save rectangle into ROI
					if (ROI.cols - ROI.rows > 87 && ROI.cols - ROI.rows < 110) {  //If cols-rows are > 87 and < 110
						rectangle(out, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 5);   //Draw rectangle
						imshow("Licence plate", ROI); //Show licence plate in new window
					}

				}
			}
		}
        imshow("Frame", out); //Show every frame 

        char c = (char)waitKey(25);   //input of key
        if (c == 27)    //27 is ESC in ASCII
            break; //If ESC is pressed, stop
    }
    cap.release();  //Release capture

	waitKey(0);
    destroyAllWindows();

    return 0;
}
