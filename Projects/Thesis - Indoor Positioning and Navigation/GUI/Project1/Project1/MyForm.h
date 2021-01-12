/****************** Realtime video captures of XO game  ***********************
Project: Indoor Positioning and Navigation of Omni-wheel Robot
Author: Anh Huy Bui - 1411408
University: Bach Khoa Ho Chi Minh
Date: June - 2018
Description: 
- GUI to handle form design and receiving data (location of robot) every timer tick
******************************************************************************/

#pragma once
#include <iostream>   // std::cout
#include <string>    
#include <msclr\marshal_cppstd.h>
#using <System.dll>
#include "stdafx.h"
#include "MyForm.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


using namespace cv;

std::string Setpoint_string;
std::string ReadBuffer;
std::string ReadBuffer_temp;
int Write_ready;
long current_location_x;
long current_location_y;
long current_location_t;
int pos_set_0;
int pos_set_1;
int pos_set_2;
int draw;
int connectedbt;
extern Mat img;
extern Mat img_temp;
extern Mat img_temp_2;
extern int set_x;
extern int set_y;
namespace Project1 {
	
	
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::Ports;
	using namespace msclr::interop;

	int check;
	std::string connected = "!";
	std::size_t char_location_x;
	std::size_t char_location_y;
	std::size_t char_location_t;
	std::string::size_type sz;
	std::string x = "x";
	std::string y = "y";
	std::string t = "t";
	int Message_check;
	
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			this->txt_Baud->Text = "38400";
			this->cb_SecCom->Text = "COM4"; 
			
			findPorts();
			
			this->Send->Enabled = false;
			this->Status->Text = "Disconnected";
			this->serialPort1->ReceivedBytesThreshold = 1;
			//this->Status->ForeColor = System::Drawing::Color->red;
			//setMouseCallback("My Window", &MyForm::CallBackFunc, NULL);
		}
		//public: void MarshalString(System::String ^ s, std::string& os);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  Pb_Connect;
	protected:

	private: System::Windows::Forms::ComboBox^  cb_SecCom;

	private: System::Windows::Forms::Timer^  timer1;
	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::Windows::Forms::TextBox^  txt_Baud;

	private: System::Windows::Forms::TextBox^  Setpoint_x;
	private: System::Windows::Forms::TextBox^  Setpoint_y;
	private: System::Windows::Forms::Button^  Send;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  Status;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  SetpointY_Label;
	private: System::Windows::Forms::Label^  SetpointX_Label;
	private: System::Windows::Forms::Label^  SetpointTheta_Label;
	private: System::Windows::Forms::TextBox^  Setpoint_t;



	private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->Pb_Connect = (gcnew System::Windows::Forms::Button());
			this->cb_SecCom = (gcnew System::Windows::Forms::ComboBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->txt_Baud = (gcnew System::Windows::Forms::TextBox());
			this->Setpoint_x = (gcnew System::Windows::Forms::TextBox());
			this->Setpoint_y = (gcnew System::Windows::Forms::TextBox());
			this->Send = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->Status = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SetpointY_Label = (gcnew System::Windows::Forms::Label());
			this->SetpointX_Label = (gcnew System::Windows::Forms::Label());
			this->SetpointTheta_Label = (gcnew System::Windows::Forms::Label());
			this->Setpoint_t = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// Pb_Connect
			// 
			this->Pb_Connect->Location = System::Drawing::Point(26, 245);
			this->Pb_Connect->Name = L"Pb_Connect";
			this->Pb_Connect->Size = System::Drawing::Size(87, 36);
			this->Pb_Connect->TabIndex = 0;
			this->Pb_Connect->Text = L"Connect";
			this->Pb_Connect->UseVisualStyleBackColor = true;
			this->Pb_Connect->Click += gcnew System::EventHandler(this, &MyForm::Pb_Connect_Click);
			// 
			// cb_SecCom
			// 
			this->cb_SecCom->FormattingEnabled = true;
			this->cb_SecCom->Location = System::Drawing::Point(16, 54);
			this->cb_SecCom->Name = L"cb_SecCom";
			this->cb_SecCom->Size = System::Drawing::Size(121, 21);
			this->cb_SecCom->TabIndex = 1;
			this->cb_SecCom->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cb_SecCom_SelectedIndexChanged);
			// 
			// timer1
			// 
			this->timer1->Interval = 20;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// serialPort1
			// 
			this->serialPort1->BaudRate = 38400;
			this->serialPort1->PortName = L"COM4";
			this->serialPort1->ReadTimeout = 500;
			this->serialPort1->WriteTimeout = 500;
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &MyForm::DataReceivedHandler);
			// 
			// txt_Baud
			// 
			this->txt_Baud->Location = System::Drawing::Point(28, 169);
			this->txt_Baud->Name = L"txt_Baud";
			this->txt_Baud->Size = System::Drawing::Size(121, 20);
			this->txt_Baud->TabIndex = 2;
			this->txt_Baud->TextChanged += gcnew System::EventHandler(this, &MyForm::txt_Baud_TextChanged);
			// 
			// Setpoint_x
			// 
			this->Setpoint_x->Location = System::Drawing::Point(213, 123);
			this->Setpoint_x->Name = L"Setpoint_x";
			this->Setpoint_x->Size = System::Drawing::Size(100, 20);
			this->Setpoint_x->TabIndex = 4;
			// 
			// Setpoint_y
			// 
			this->Setpoint_y->Location = System::Drawing::Point(213, 169);
			this->Setpoint_y->Name = L"Setpoint_y";
			this->Setpoint_y->Size = System::Drawing::Size(100, 20);
			this->Setpoint_y->TabIndex = 4;
			this->Setpoint_y->TextChanged += gcnew System::EventHandler(this, &MyForm::Setpoint_y_TextChanged);
			// 
			// Send
			// 
			this->Send->Location = System::Drawing::Point(213, 314);
			this->Send->Name = L"Send";
			this->Send->Size = System::Drawing::Size(100, 36);
			this->Send->TabIndex = 0;
			this->Send->Text = L"Send";
			this->Send->UseVisualStyleBackColor = true;
			this->Send->Click += gcnew System::EventHandler(this, &MyForm::Send_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->label1->Font = (gcnew System::Drawing::Font(L"Cambria", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(49, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(264, 28);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Điều khiển robot Omni";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// Status
			// 
			this->Status->AutoSize = true;
			this->Status->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Status->ForeColor = System::Drawing::Color::Red;
			this->Status->Location = System::Drawing::Point(49, 182);
			this->Status->Name = L"Status";
			this->Status->Size = System::Drawing::Size(51, 16);
			this->Status->TabIndex = 6;
			this->Status->Text = L"label2";
			this->Status->TextChanged += gcnew System::EventHandler(this, &MyForm::Status_TextChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->Status);
			this->groupBox1->Controls->Add(this->Pb_Connect);
			this->groupBox1->Controls->Add(this->cb_SecCom);
			this->groupBox1->Location = System::Drawing::Point(12, 69);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(165, 293);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Bluetooth Connection";
			this->groupBox1->Enter += gcnew System::EventHandler(this, &MyForm::groupBox1_Enter);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(16, 84);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(58, 13);
			this->label5->TabIndex = 8;
			this->label5->Text = L"Baudrate";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(16, 37);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(34, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"COM";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->ForeColor = System::Drawing::SystemColors::Desktop;
			this->label3->Location = System::Drawing::Point(13, 154);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(69, 16);
			this->label3->TabIndex = 7;
			this->label3->Text = L"STATUS";
			// 
			// SetpointY_Label
			// 
			this->SetpointY_Label->AutoSize = true;
			this->SetpointY_Label->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->SetpointY_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SetpointY_Label->Location = System::Drawing::Point(210, 153);
			this->SetpointY_Label->Name = L"SetpointY_Label";
			this->SetpointY_Label->Size = System::Drawing::Size(66, 13);
			this->SetpointY_Label->TabIndex = 9;
			this->SetpointY_Label->Text = L"Setpoint Y";
			// 
			// SetpointX_Label
			// 
			this->SetpointX_Label->AutoSize = true;
			this->SetpointX_Label->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->SetpointX_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SetpointX_Label->Location = System::Drawing::Point(210, 106);
			this->SetpointX_Label->Name = L"SetpointX_Label";
			this->SetpointX_Label->Size = System::Drawing::Size(66, 13);
			this->SetpointX_Label->TabIndex = 9;
			this->SetpointX_Label->Text = L"Setpoint X";
			// 
			// SetpointTheta_Label
			// 
			this->SetpointTheta_Label->AutoSize = true;
			this->SetpointTheta_Label->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->SetpointTheta_Label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->SetpointTheta_Label->Location = System::Drawing::Point(210, 201);
			this->SetpointTheta_Label->Name = L"SetpointTheta_Label";
			this->SetpointTheta_Label->Size = System::Drawing::Size(91, 13);
			this->SetpointTheta_Label->TabIndex = 11;
			this->SetpointTheta_Label->Text = L"Setpoint Theta";
			// 
			// Setpoint_t
			// 
			this->Setpoint_t->Location = System::Drawing::Point(213, 217);
			this->Setpoint_t->Name = L"Setpoint_t";
			this->Setpoint_t->Size = System::Drawing::Size(100, 20);
			this->Setpoint_t->TabIndex = 10;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(346, 399);
			this->Controls->Add(this->SetpointTheta_Label);
			this->Controls->Add(this->Setpoint_t);
			this->Controls->Add(this->SetpointX_Label);
			this->Controls->Add(this->SetpointY_Label);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Setpoint_y);
			this->Controls->Add(this->Setpoint_x);
			this->Controls->Add(this->txt_Baud);
			this->Controls->Add(this->Send);
			this->Controls->Add(this->groupBox1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: void findPorts(void)
	{
		// get port names
		array<Object^>^ objectArray = SerialPort::GetPortNames();
		// add string array to combobox
		this->cb_SecCom->Items->AddRange(objectArray);

	}
	private: System::Void Pb_Connect_Click(System::Object^  sender, System::EventArgs^  e) {
		if (!this->serialPort1->IsOpen) {
			this->serialPort1->PortName = this->cb_SecCom->Text;
			this->serialPort1->BaudRate = Int32::Parse((this->txt_Baud->Text));
			try
			{
				this->serialPort1->Open();
				this->Status->Text = "Connecting";
			}
			catch (IO::IOException^ e)
			{
				this->Status->Text = "Port not open \n Retrying";
				try
				{
					this->serialPort1->PortName = this->cb_SecCom->Text;
					this->serialPort1->BaudRate = Int32::Parse((this->txt_Baud->Text));
					this->serialPort1->Open();
					this->Status->Text = "Connecting";
				}
				catch (IO::IOException^ e)
				{
					this->Status->Text = "Port not open \n Check device";
				}
			}
			//open serial port 

			this->timer1->Start();
			if (this->serialPort1->IsOpen) this->serialPort1->Write("!#");
			else {
				this->Status->Text = "Port not open";
			}
			check = 1;
		}
		else
			this->Status->Text = "Port isn't openned";

	}

	private: System::Void Send_Click(System::Object^  sender, System::EventArgs^  e) {
		if (this->serialPort1->IsOpen) {
			if ((this->Setpoint_x->Text != "") && (this->Setpoint_y->Text != "")) {
				System::String^ Setpoint = 
					("x" + "+" + this->Setpoint_x->Text +"y"+ "+" + this->Setpoint_y->Text+"t"+"+"+this->Setpoint_t->Text+"#");
				this->serialPort1->Write(Setpoint);
			}
		}
	}
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
		if (check == 1)
		{
			static int t = 2;
			t--;
				if ((t == 0) && (check == 1)) this->Status->Text = "Port not open\n  Check device";
		}
		if (Write_ready == 1) {
			System::String^ WriteBuffer = (gcnew System::String(Setpoint_string.c_str())) + "t" + "+" + "000" + "#";
			this->Setpoint_x->Text = System::Convert::ToString(set_x);
			this->Setpoint_y->Text = System::Convert::ToString(set_y);
			this->Setpoint_t->Text = "000";
			if ((this->serialPort1->IsOpen) && (WriteBuffer->Length == 16)){ 
				this->serialPort1->Write(WriteBuffer);
			}
		}
 		if (Message_check == 1) {

			char_location_x = ReadBuffer.find(x);
			if (char_location_x <= ReadBuffer.length())
			{
				try
				{
					ReadBuffer_temp = ReadBuffer.substr(char_location_x + 1, 4);
					std::istringstream iss(ReadBuffer_temp);
					long num;
					if (!(iss >> num).fail()) {
						try {
							current_location_x = std::stol(ReadBuffer_temp, 0);
							draw++;
						}
						catch (std::invalid_argument)
						{
							draw--;
						};
					}
				}
				catch(std::out_of_range& exception){}
			}
			char_location_y = ReadBuffer.find(y);
			if (char_location_y <= ReadBuffer.length())
			{
				try {
					ReadBuffer_temp = ReadBuffer.substr(char_location_y + 1, 4);
					std::istringstream iss(ReadBuffer_temp);
					long num;
					if (!(iss >> num).fail()) {
						try {
							current_location_y = std::stol(ReadBuffer_temp, 0);
							draw++;
						}
						catch (std::invalid_argument)
						{
							draw--;
						};
					}
				}
				catch (std::out_of_range& exception) {}
			}
			char_location_t = ReadBuffer.find(t);
			if (char_location_t <= ReadBuffer.length())
			{
				try {
					ReadBuffer_temp = ReadBuffer.substr(char_location_t + 1, 4);
					std::istringstream iss(ReadBuffer_temp);
					long num;
					if (!(iss >> num).fail()) {
						try {
							current_location_t = std::stol(ReadBuffer_temp, 0);
							draw++;
						}
						catch (std::invalid_argument)
						{
							draw--;
						};
					}
				}
				catch (std::out_of_range& exception) {}
			}
			Point2f point_2d = Point2f((float)(current_location_x*2 + img.cols/2), (float)(img.rows/2 - 2*current_location_y));
			if (1) {//draw == 3) {
				circle(img, point_2d, 3, Scalar(0, 0, 255), FILLED, LINE_8);
				circle(img_temp, point_2d, 3, Scalar(0, 0, 255), FILLED, LINE_8);
				std::string position = "Position: {" + std::to_string(current_location_x) + ", " + std::to_string(current_location_y) + ", " + std::to_string(current_location_t/100)+"."+std::to_string((current_location_t %100)/10)+ std::to_string(current_location_t %10) + "}";
				img_temp.copyTo(img_temp_2);
				putText(img_temp_2, position, Point2f(30, 710), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 0.2, LINE_AA);
			}
			draw = 0;
			imshow("My Window", img_temp_2);
		}
	}
	private: System::Void txt_Baud_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: void  DataReceivedHandler(Object^ sender, SerialDataReceivedEventArgs^ e) {
		ReadBuffer = "";
		ReadBuffer_temp = "";

		SerialPort^ sp = (SerialPort^)sender;
		System::String^ indata = sp->ReadExisting();
		Console::Write(indata);
		serialPort1->DiscardInBuffer();
		MarshalString(indata, ReadBuffer);
		if (ReadBuffer.size() == 15)
			Message_check = 1;
		else Message_check = 0;

		if (check == 1)
		{
			if (ReadBuffer.compare(connected) == 0)
			{
				Connected("Connected");
				check = 0;
				connectedbt = 1;
				this->serialPort1->ReceivedBytesThreshold = 15;
				return;
			}
		}

	}
	private: delegate void ConnectedCallBack(System::String ^ text);
	private: void Connected(System::String ^ text) {
		if (this->Status->InvokeRequired)
		{
			//ConnectedCallBack^ d = gcnew ConnectedCallBack(this,MyForm::Connected);
			array<Object^>^myEnumArray = { text };
			this->Invoke(gcnew ConnectedCallBack(this, &MyForm::Connected), myEnumArray);
		}
		else
		{
			this->Status->Text = text;
		}

	}

	private: void MarshalString(System::String ^ s, std::string& os)
	{
		using namespace Runtime::InteropServices;
		const char* chars =
			(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}

	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void Setpoint_y_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void cb_SecCom_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void groupBox1_Enter(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void Status_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		if (this->Status->Text == "Connected") {
			this->Send->Enabled = true;
			this->Status->ForeColor = System::Drawing::Color::Green;
		}
		
	}
};
}

