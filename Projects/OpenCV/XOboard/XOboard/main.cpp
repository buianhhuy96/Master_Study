/****************** Realtime video captures of XO game  ***********************
Project: Realtime video captures of XO game
Author: Anh Huy Bui - 1411408
University: Bach Khoa Ho Chi Minh
Date: October - 2017
Description: 
- Input: Camera, X-O game on paper.
- Output: Realtime version of X-O game drawn on computer.
+ This project uses camera to record 2 persons playing X-O game on paper, analyzes 
and redraw an digital version of the game simuntanceouly. 
+ Camera connects to PC wirely or wirelessly via an IP address.
+ OpenCV library is used to 
-> read video frame 
-> detect game board borders/edges and corners
-> reconstruct the frame with mapped board corners so that it can be shaped as a rectangle
-> count number of squares inside game board
-> detect X-O inside each square using an already-made mask of X and O
******************************************************************************/

#include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;
using namespace cv;

Mat src, src_g, src_b;
Mat M, board, board_g, board_b;
Mat draw, drawX, drawO;
int Threshold = 128;
int board_rows = 0, board_cols = 0;
int square_count = 0;
float square_w, square_h;
int level1 = 10, level2 = 150;
int setup_flag = 0, flag = -1;
int calibLine = 4;
int sum_x = 0, sum_y = 0;
vector<vector<Point>> contours;
vector<Point> border;
vector<Point> vertices;
vector<vector<Point>> squares(200, vector<Point>(4));
void SetupBackground();
void DetectBorder();
void DetectSquares();
void SetupDraw();
void redrawBoard();
void getBoard();
void processSquare(vector<Point> square_corners);
int Diagonal(vector<Point> rect);
int findXO(Mat img);
int findXO2(Mat img);
float findblack(Mat img);

int main()
{
	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened())  // Connect to camera
	{
		cout << "Cannot open the webcam" << endl;					
		return -1;
	}
	bool bSuccess = cap.read(src); // Read a frame
	if (!bSuccess)
	{
		cout << "Cannot read a frame from video stream" << endl;	
		return -1;
	}
	//src = imread("xoil.jpg");
	SetupBackground();
	DetectBorder();
	DetectSquares();
	SetupDraw();
	while (waitKey(50) != 'q')
	{	
		bool bSuccess = cap.read(src); // Read a frame
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;	
			return -1;
		}
		getBoard();
		redrawBoard();
		for (size_t i = 0; i < square_count; i++)
		{
			try
			{
				processSquare(squares[i]);
			}
			catch (int e)
			{

			}
		}
		imshow("Draw", draw);
	}
	return 0;
}

/* Adjust threshold to get clear XO board */
void SetupBackground()
{
	namedWindow("SetupBackground", WINDOW_NORMAL);
	createTrackbar("Threshold", "SetupBackground", &Threshold, 255);
	while (waitKey(20) != 13) // If OK -> Enter
	{
		cvtColor(src, src_g, CV_BGR2GRAY);
		medianBlur(src_g, src_g, 1);
		threshold(src_g, src_b, Threshold, 255, THRESH_BINARY);

		imshow("SetupBackground", src_b);
	}
	destroyWindow("SetupBackground");
}

/* Detect board borders, cut and stretch them out */
void DetectBorder()
{
	int diag, diag_max = 0;
	int diag_src = sqrt(src.cols*src.cols + src.rows*src.rows);
	findContours(src_b, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(contours[i], vertices, arcLength(contours[i], 1)*0.02, 1);
		/* Check if picking a quadrilateral */
		if (vertices.size() == 4)
		{
			diag = Diagonal(vertices);
			/* Pick the largest quadrilateral */
			if ((diag>diag_max) && (diag<diag_src - 20))
			{
				diag_max = diag;
				border = vertices;
			}
		}
	}
	line(src, border[0], border[1], cvScalar(255, 0, 0), 4);
	line(src, border[1], border[2], cvScalar(0, 255, 0), 4);
	line(src, border[2], border[3], cvScalar(0, 255, 0), 4);
	line(src, border[3], border[0], cvScalar(0, 255, 255), 4);
	/*namedWindow("DetectBorder", WINDOW_NORMAL);
	while (waitKey(50) != 13)
	{

		imshow("DetectBorder", src);
	}
	destroyWindow("DetectBorder");*/
	/* Transform border */
	Point2f inputQuad[4], outputQuad[4];
	for (size_t i = 0; i < 4; i++)
	{
		inputQuad[i] = Point2f(border[i]);
		/* Get pixel location of 4 corners*/
		if (inputQuad[i].x > src.cols / 2) outputQuad[i].x = src.cols;
		else outputQuad[i].x = 0;
		if (inputQuad[i].y > src.rows / 2) outputQuad[i].y = src.rows;
		else outputQuad[i].y = 0;
	}
	bool Check_b = false;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (i != j)
			{
				/* Check if picking duplicated corners */
				if ((outputQuad[i].x == outputQuad[j].x) &&  \
				    (outputQuad[i].y == outputQuad[j].y))
					 Check_b=true;
			}
		}
	}
	if (!Check_b)
	{
		M = getPerspectiveTransform(inputQuad, outputQuad);
		warpPerspective(src, board, M, board.size());
	}
}

/* Detect number of squares in board, in each col, in each row, squares's width and squares's height */
void DetectSquares() 
{

	int x, y;
 	cvtColor(board, board_g, CV_BGR2GRAY);
	threshold(board_g, board_b, Threshold, 255, CV_THRESH_BINARY);
	namedWindow("DetectBo", WINDOW_NORMAL);
	imshow("DetectBo", board_g);
	findContours(board_b, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(contours[i], vertices, arcLength(contours[i], 1)*0.02, 1);
		if (vertices.size() == 4)
		{
			x = abs(vertices[0].x - vertices[2].x);
			y = abs(vertices[0].y - vertices[2].y);
			if ((x>5) && (y > 5) && (x<board.cols - 10) && (y<board.rows - 10))
			{
				squares[square_count] = vertices;
				square_count++;
				sum_x += x;
				sum_y += y;
				line(board, vertices[0], vertices[1], cvScalar(255, 255, 0), 4);
				line(board, vertices[1], vertices[2], cvScalar(0, 255, 255), 4);
				line(board, vertices[2], vertices[3], cvScalar(0, 255, 255), 4);
				line(board, vertices[3], vertices[0], cvScalar(100, 255, 255), 4);
			}

		}
	}
	square_w = (float)(sum_y + calibLine) / square_count;
	square_h = (float)(sum_x + calibLine) / square_count;
	board_rows = board.rows / square_w;
	board_cols = board.cols / square_h;
	namedWindow("DetectSquares", WINDOW_NORMAL);
	while (waitKey(50) != 13)
	{

		imshow("DetectSquares", board);
	}
	destroyWindow("DetectSquares");
}

void SetupDraw()
{
	drawX = imread("X.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	drawO = imread("O.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("Draw", WINDOW_AUTOSIZE);
	createTrackbar("Calib line", "Draw", &calibLine, 500);
	createTrackbar("Level X", "Draw", &level1, 100);
	createTrackbar("Level O", "Draw", &level2, 300);

	while (waitKey(20) != 13) // If OK -> Enter
	{
		board_rows = board.rows / (square_w + calibLine);
		board_cols = board.cols / (square_h + calibLine);
		draw = Mat::ones(board_rows * 50, board_cols * 50, CV_8U)*255.;
		for (int i = 1; i < board_cols; i++)
			line(draw, Point2f(i*(draw.cols / board_cols), 0),  \
			           Point2f(i*(draw.cols / board_cols), draw.rows), (0, 0, 0), 5, 8, 0);
		for (int i = 1; i < board_rows; i++)
			line(draw, Point2f(0, i*(draw.rows / board_rows)),  \
			           Point2f(draw.cols, i*(draw.rows / board_rows)), (0, 0, 0), 5, 8, 0);
		imshow("Draw", draw);
	}

}

void redrawBoard()
{
	cvtColor(src, src_g, CV_BGR2GRAY);
	threshold(src_g, src_b, Threshold, 255, THRESH_BINARY);
	draw = Mat::ones(board_rows * 50, board_cols * 50, CV_8U)*255.;
	for (int i = 1; i < board_cols; i++)
		line(draw, Point2f(i*(draw.cols / board_cols), 0),  \
				   Point2f(i*(draw.cols / board_cols), draw.rows), (0, 0, 0), 5, 8, 0);
	for (int i = 1; i < board_rows; i++)
		line(draw, Point2f(0, i*(draw.rows / board_rows)),  \
		           Point2f(draw.cols, i*(draw.rows / board_rows)), (0, 0, 0), 5, 8, 0);
}

void getBoard()
{
	cvtColor(src, src_g, CV_BGR2GRAY);
	medianBlur(src_g, src_g, 1);
	threshold(src_g, src_b, Threshold, 255, THRESH_BINARY);
	DetectBorder();
	warpPerspective(src, board, M, board.size());
	namedWindow("Board", CV_WINDOW_NORMAL);
	imshow("Board", board);
}

void processSquare(vector<Point> square_corners) // Get the squares from 4 point
{
	square_w = board.rows / board_rows;
	square_h = board.cols / board_cols;
	float xMed, yMed;
	// Draw lines on square borders
	line(board, square_corners[0], square_corners[1], cvScalar(255, 0, 0), 4);
	line(board, square_corners[1], square_corners[2], cvScalar(0, 255, 0), 4);
	line(board, square_corners[2], square_corners[3], cvScalar(0, 255, 0), 4);
	line(board, square_corners[3], square_corners[0], cvScalar(0, 255, 255), 4);

	int xMax = square_corners[2].x, xMin = square_corners[0].x;
	int yMax = square_corners[2].y, yMin = square_corners[0].y;
	if (square_corners[0].x > square_corners[2].x)
	{
		xMax = square_corners[0].x;
		xMin = square_corners[2].x;
	}
	if (square_corners[0].y > square_corners[2].y)
	{
		yMax = square_corners[0].y;
		yMin = square_corners[2].y;
	}
	// Extract square from board
	Mat temp = board(Range(yMin, yMax), Range(xMin, xMax));
	cvtColor(temp, temp, CV_BGR2GRAY);
	// Square center
	xMed = (float)(xMax + xMin) / 2;
	yMed = (float)(yMax + yMin) / 2;
	if ((yMed / square_w  < board_rows) && (xMed / square_h < board_cols))
	{
		Mat temp_draw = draw(Range(floor(yMed/square_w)*50, (floor(yMed/square_w)+1)*50),   \
		                     Range(floor(xMed/square_h)*50, (floor(xMed/square_h)+1)*50));
		flag = findXO(temp);
		if (flag == 1) bitwise_and(temp_draw, drawX, temp_draw);
		else if (flag == 0) bitwise_and(temp_draw, drawO, temp_draw);
	}
}

int findXO(Mat img)
{	
	int rec = -1;
	float t = (float)findblack(img) / (img.cols*img.rows) - (float)level1/1000;
	if (t>0)
	{
		int check = 0;
		
		Mat modelx = imread("xmask.jpg", CV_8UC1);
		Mat modelo = imread("omask.jpg", CV_8UC1);
		resize(modelx, modelx, Size(img.cols, img.rows), 1, 1, INTER_LINEAR);
		resize(modelo, modelo, Size(img.cols, img.rows), 1, 1, INTER_LINEAR);

		//float initx = findblack(modelx);
		float init = findblack(img);

		modelx = modelx + img;
		modelo = modelo + img;
		float kx = findblack(modelx) / init;
		float ko = findblack(modelo) / init;
		if (kx > ko)  rec = 1;
		else if (kx < ko) rec = 0;
	}
	else rec = -1;
	return rec;
}

float findblack(Mat img)
{

	float count = 0;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (img.at<uchar>(i, j) < Threshold) count++;
		}
	}
	return count;
}

int Diagonal(vector<Point> rect) /* Find the biggest diagonal */
{
	int Diagonal_1 = sqrt((rect[0].x - rect[2].x)*(rect[0].x - rect[2].x) + \
						  (rect[0].y - rect[2].y)*(rect[0].y - rect[2].y));
	int Diagonal_2 = sqrt((rect[1].x - rect[3].x)*(rect[1].x - rect[3].x) + \
						  (rect[1].y - rect[3].y)*(rect[1].y - rect[3].y));
	if (Diagonal_1 > Diagonal_2) return Diagonal_1;
	else return Diagonal_2;
}

