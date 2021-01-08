#include "stdafx.h"
#include "MyForm.h"
/*#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>*/
#include <iostream>

using namespace std;
using namespace cv;
using namespace Project1;
extern std::string Setpoint_string;
extern int Write_ready;
extern std::string ReadBuffer;
extern long pos_0;
extern long pos_1;
extern long pos_2;
extern int pos_set_0;
extern int pos_set_1;
extern int pos_set_2;
int set_x;
int set_y;
Mat img,img_temp, img_temp_2;
extern int connectedbt;
void MarshalStringS(System::String ^ s, std::string& os)
{
	using namespace Runtime::InteropServices;
	const char* chars =
		(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (connectedbt == 1) {
		System::String^ Setpointx;
		System::String^ Setpointy;
		System::String^ Setpointt;
		System::String^ Setpoint;
		if (event == EVENT_LBUTTONDOWN)
		{
			Point2f point_2d = Point2f((float)(x), (float)(y));
			circle(img, point_2d, 3, Scalar(255, 0, 0), FILLED, LINE_8);
			//putText(img, "Setpoint" , (10, 500), FONT_HERSHEY_SIMPLEX, 4, Scalar(255, 0, 0), 2, cv2.LINE_AA);
			pos_set_0 = (x - img.cols/2) /(img.cols / 400);
			pos_set_1 = (-y + img.rows/2) /(img.rows / 400);
			set_x = pos_set_0;
			set_y = pos_set_1;
		std:string setpoint = "Setpoint: {" + std::to_string(pos_set_0) + ", " + std::to_string(pos_set_1) + ", 0}";
			img.copyTo(img_temp);
			putText(img_temp, setpoint, Point2f(30, 695), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 0.2, LINE_AA);
			if (abs(pos_set_0) < 100) {
				if (abs(pos_set_0) < 10)
				{
					if (pos_set_0 < 0) Setpointx = ("x" + "-" + "0" + "0" + System::Convert::ToString(abs(pos_set_0)));
					else Setpointx = ("x" + "+" + "0" + "0" + System::Convert::ToString(abs(pos_set_0)));
				}
				else
				{
					if (pos_set_0 < 0) Setpointx = ("x" + "-" + "0" + System::Convert::ToString(abs(pos_set_0)));
					else Setpointx = ("x" + "+" + "0" + System::Convert::ToString(abs(pos_set_0)));
				}
			}
			else {
				if (pos_set_0 < 0) Setpointx = ("x" + "-" + System::Convert::ToString(abs(pos_set_0)));
				else Setpointx = ("x" + "+" + System::Convert::ToString(abs(pos_set_0)));
			}
			if (abs(pos_set_1) < 100) {
				if (abs(pos_set_1) < 10)
				{
					if (pos_set_1 < 0) Setpointy = ("y" + "-" + "0" + "0" + System::Convert::ToString(abs(pos_set_1)));
					else Setpointy = ("y" + "+" + "0" + "0" + System::Convert::ToString(abs(pos_set_1)));
				}
				else {
					if (pos_set_1 < 0) Setpointy = ("y" + "-" + "0" + System::Convert::ToString(abs(pos_set_1)));
					else Setpointy = ("y" + "+" + "0" + System::Convert::ToString(abs(pos_set_1)));
				}
			}
			else {
				if (pos_set_1 < 0) Setpointy = ("y" + "-" + System::Convert::ToString(abs(pos_set_1)));
				else Setpointy = ("y" + "+" + System::Convert::ToString(abs(pos_set_1)));
			}
			Setpoint = Setpointx + Setpointy;
			MarshalStringS(Setpoint, Setpoint_string);
			Write_ready = 1;
		}
		else if (event == EVENT_RBUTTONDOWN)
		{
			cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		}
		else if (event == EVENT_MBUTTONDOWN)
		{
			cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		}
		else if (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_ALTKEY)
		{
			Point2f point_2d = Point2f((float)(x), (float)(y));
			circle(img, point_2d, 3, Scalar(255, 0, 0), FILLED, LINE_8);
			img.copyTo(img_temp);
			std::string setpoint = "Setpoint: {" + std::to_string(pos_set_0) + ", " + std::to_string(pos_set_1) + ", 0}";
			putText(img_temp, setpoint, Point2f(30, 695), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 0.2, LINE_AA);
			//Console::Write(System::Convert::ToString(x) + System::Convert::ToString(y));
			pos_set_0 = (x - img.cols/2)/(img.cols/400);
			pos_set_1 = (-y + img.rows/2)/(img.rows/ 400);
			if (abs(pos_set_0) < 100) {
				if (abs(pos_set_0) < 10)
				{
					if (pos_set_0 < 0) Setpointx = ("x" + "-" + "0" + "0" + System::Convert::ToString(abs(pos_set_0)));
					else Setpointx = ("x" + "+" + "0" + "0" + System::Convert::ToString(abs(pos_set_0)));
				}
				else
				{
					if (pos_set_0 < 0) Setpointx = ("x" + "-" + "0" + System::Convert::ToString(abs(pos_set_0)));
					else Setpointx = ("x" + "+" + "0" + System::Convert::ToString(abs(pos_set_0)));
				}
			}
			else {
				if (pos_set_0 < 0) Setpointx = ("x" + "-" + System::Convert::ToString(abs(pos_set_0)));
				else Setpointx = ("x" + "+" + System::Convert::ToString(abs(pos_set_0)));
			}
			if (abs(pos_set_1) < 100) {
				if (abs(pos_set_1) < 10)
				{
					if (pos_set_1 < 0) Setpointy = ("y" + "-" + "0" + "0" + System::Convert::ToString(abs(pos_set_1)));
					else Setpointy = ("y" + "+" + "0" + "0" + System::Convert::ToString(abs(pos_set_1)));
				}
				else {
					if (pos_set_1 < 0) Setpointy = ("y" + "-" + "0" + System::Convert::ToString(abs(pos_set_1)));
					else Setpointy = ("y" + "+" + "0" + System::Convert::ToString(abs(pos_set_1)));
				}
			}
			else {
				if (pos_set_1 < 0) Setpointy = ("y" + "-" + System::Convert::ToString(abs(pos_set_1)));
				else Setpointy = ("y" + "+" + System::Convert::ToString(abs(pos_set_1)));
			}
			Setpoint = Setpointx + Setpointy;
			MarshalStringS(Setpoint, Setpoint_string);
			Write_ready = 1;
		}
	}
}

void main()
{
	img = imread("Map3.PNG");
	namedWindow("My Window", WINDOW_NORMAL);
	img.copyTo(img_temp);
	img.copyTo(img_temp_2);
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	setMouseCallback("My Window", CallBackFunc, NULL);
	// Create the main window and run it
	while (1)
	{
				/*char_pos_x = ReadBuffer.find(x);
				if (char_pos_x != -1)
				{
					ReadBuffer_temp.erase(0, char_pos_x + 1);
					ReadBuffer_temp.erase(0 + 3, 100);
					pos_0 = std::stoi(ReadBuffer_temp, &sz);
					ReadBuffer_temp = ReadBuffer;
				}
				char_pos_y = ReadBuffer.find(y);
				if (char_pos_y != -1)
				{
					ReadBuffer_temp.erase(0, char_pos_y + 1);
					ReadBuffer_temp.erase(0 + 3, 100);
					pos_1 = std::stoi(ReadBuffer_temp, &sz);
					ReadBuffer_temp = ReadBuffer;
				}
				char_pos_t = ReadBuffer.find(t);
				if (char_pos_t != -1)
				{
					ReadBuffer_temp.erase(0, char_pos_t + 1);
					ReadBuffer_temp.erase(0 + 3, 100);
					pos_2 = std::stoi(ReadBuffer_temp, &sz);
					ReadBuffer_temp = ReadBuffer;
				}*/
	
	
		
		
	
		Application::Run(gcnew MyForm());
		waitKey(0);
	}
	return;
}