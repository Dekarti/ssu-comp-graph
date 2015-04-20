#pragma once

#include <string>
#include "PClip.h"
#define HEIGHT this->ClientRectangle.Height
#define WIDTH this->ClientRectangle.Width

namespace test3v11 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
	private: System::Windows::Forms::Button^  btnOpen;

	private: System::Collections::Generic::List<polygon3D^> triangles;
			 System::Collections::Generic::List<Color> clrs;
			 Color curColor;

	private: float left, right, top, bottom;
			 float Wcx, Wcy, Wx, Wy;
			 float Vcx, Vcy, Vx, Vy;
			 float far;
			 float aspect, aspectDefault;
			 float near  , nearDefault;
			 float fovy  , fovyDefault;
			 point3D center;
			 point3D eye, eyeDefault;
			 point3D up , upDefault;
			 bool prOrtho;



#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Required method for Designer support - do not modify
			 /// the contents of this method with the code editor.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
				 this->btnOpen = (gcnew System::Windows::Forms::Button());
				 this->SuspendLayout();
				 // 
				 // openFileDialog
				 // 
				 this->openFileDialog->DefaultExt = L"txt";
				 this->openFileDialog->FileName = L"openFileDialog1";
				 this->openFileDialog->Filter = L"Текстовые файлы (*.txt)|*.txt|Все файлы (*.*)|*.*";
				 this->openFileDialog->Title = L"Открыть файл";
				 this->openFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openFileDialog1_FileOk);
				 // 
				 // btnOpen
				 // 
				 this->btnOpen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
				 this->btnOpen->Location = System::Drawing::Point(860, 12);
				 this->btnOpen->Name = L"btnOpen";
				 this->btnOpen->Size = System::Drawing::Size(75, 23);
				 this->btnOpen->TabIndex = 0;
				 this->btnOpen->Text = L"Открыть";
				 this->btnOpen->UseVisualStyleBackColor = true;
				 this->btnOpen->Click += gcnew System::EventHandler(this, &Form1::btnOpen_Click);
				 // 
				 // Form1
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(984, 661);
				 this->Controls->Add(this->btnOpen);
				 this->DoubleBuffered = true;
				 this->KeyPreview = true;
				 this->MinimumSize = System::Drawing::Size(500, 350);
				 this->Name = L"Form1";
				 this->Text = L"Form1";
				 this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
				 this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::Form1_Paint);
				 this->DockChanged += gcnew System::EventHandler(this, &Form1::btnOpen_Click);
				 this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
				 this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyDown);
				 this->ResumeLayout(false);

			 }
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->Refresh();
				 prOrtho = true;
				 left = right = top = bottom = 60;

				 Wcx = left;
				 Wcy = HEIGHT - bottom;
				 Wx = WIDTH - left - right;
				 Wy = HEIGHT - top - bottom;

				 upDefault.x = 0;
				 upDefault.y = 1;
				 upDefault.z = 0;

				 center.x = 0;
				 center.y = 0;
				 center.z = 0;

				 aspectDefault = Wx / Wy;
				 aspect = aspectDefault;

				 triangles.Clear();
			 }
	private: System::Void Form1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 Graphics^ g = e->Graphics;
				 g->Clear(Color::White);
				 Pen^ pen = gcnew Pen(Color::Black, 1);
				 Pen^ rectPen  = gcnew Pen(Color::Cyan , 8);

				 point Pmax, Pmin;
				 Pmax.x = WIDTH  - right;
				 Pmin.x = left;

				 Pmax.y = HEIGHT - bottom;
				 Pmin.y = top;
				 g->DrawRectangle(rectPen, left, top, Wx, Wy);

				 mat3D T, V, U, R;
				 unit(T);
				 unit(V);
				 unit(U);
				 unit(R);

				 LookAt(eye, center, up, V);
				 if (prOrtho) {
				 	 Vy = 2 * near * tan(fovy / 2);
					 Vx = aspect * Vy;
					 Ortho(Vx, Vy, near, far, U);
				 }
				 else {
					 Perspective(fovy, aspect, near, far, U);
				 }
				 mat3D VT;
				 times(V, T, VT);
				 times(U, VT, R);

				 mat F;
				 frame(2, 2, -1, -1, Wx, Wy, Wcx, Wcy, F);

				 for (int i = 0; i < triangles.Count; i++) {
					 polygon3D^ p = triangles[i];
					 polygon^ p1 = gcnew polygon(0);
					 point3D A;
					 point A1, A2, c;
					 vec3D a, a1;
					 vec a2, a3;

					 for (int j = 0; j < p->Count; j++) {
						 point2vec(p[j], a);
						 timesMatVec(R, a, a1);
						 vec2point(a1, A);
						 set(A, A1);

						 point2vec(A1, a2);
						 timesMatVec(F, a2, a3);
						 vec2point(a3, A2);
						 p1->Add(A2);
					 }
					 p1 = Pclip(p1, Pmin, Pmax);
					 if (p1->Count) {
						 pen->Color = clrs[i];
						 c = p1[p1->Count - 1];
						 for (int j = 0; j < p1->Count; j++) {
							 g ->DrawLine(pen, c.x, c.y, p1[j].x, p1[j].y);
							 c = p1[j];
						 }
					 }
				 }
			 }
	private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {

			 }
	private: System::Void btnOpen_Click(System::Object^  sender, System::EventArgs^  e) {
				 triangles.Clear();
				 if (this->openFileDialog->ShowDialog() ==
					 System::Windows::Forms::DialogResult::OK) {
						 wchar_t fileName[1024];
						 for (int i = 0; i < openFileDialog->FileName->Length; i++) {
							 fileName[i] = openFileDialog->FileName[i];
						 }
						 fileName[openFileDialog->FileName->Length] = '\0';
						 std::ifstream in;
						 in.open(fileName);

						 if (in.is_open()) {
							 matrices.clear();
							 std::stack<mat> matStack;
							 mat K;
							 unit(K);
							 std::string str;
							 getline(in, str);
							 while (in) {
								 if(str.find_first_not_of(" \t\r\n") != std::string::npos
									 && str[0] != '#') {
										 std::stringstream s(str);
										 std::string cmd;
										 s >> cmd;
										 if (cmd == "triangle") {
											 point3D a, b, c;
											 s >> a.x >> a.y >> a.z;
											 s >> b.x >> b.y >> b.z;
											 s >> c.x >> c.y >> c.z;
											 polygon3D^ triangle = gcnew polygon3D(0);
											 triangle->Add(a);
											 triangle->Add(b);
											 triangle->Add(c);
											 triangles.Add(triangle);
											 clrs.Add(curColor);
										 }
										 else if (cmd == "lookat") {
											 float x1, y1, z1;
											 float x2, y2, z2;
											 float x3, y3, z3;

											 s >> x1 >> y1 >> z1;
											 s >> x2 >> y2 >> z2;
											 s >> x3 >> y3 >> z3;

											 eyeDefault.x = x1; eyeDefault.y = y1; eyeDefault.z = z1;
											 center.x     = x2; center.y	 = y2; center.z     = z2;
											 upDefault.x  = x3; upDefault.y  = y3; upDefault.z  = z3;
										 }
										 else if (cmd == "color") {
											 float R, G, B;
											 s >> R
											   >> G
											   >> B;

											 curColor = Color::FromArgb(R, G, B);
										 }
										 else if (cmd == "screen") {
											 float fD, nD, f;
											 s >> fD >> nD >> f;

											 fovyDefault = fD;
											 nearDefault = nD;
											 far = f;
										 }


								 }
								 getline(in, str);
							 }
						 }

						 near = nearDefault; 
						 fovy = fovyDefault;

						 eye = eyeDefault;
						 up  = upDefault;
						 this->Refresh();
				 }
			 }
	private: System::Void Form1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				 mat R, T1;
				 switch(e->KeyCode){
					case Keys::P :
						prOrtho ^= 1;
					case Keys::Escape :
						break;
					default :
						unit(R);
				 }
				 this->Refresh();
			 }
	private: System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e) {
				 float oldWx = Wx,
					   oldWy = Wy;


				 Wcy = HEIGHT - bottom;
				 Wx = WIDTH - left - right;
				 Wy = HEIGHT - top - bottom;
				 aspect = Wx / Wy;

				 mat R, T1;

				 move(-Wcx, -top, R);
				 times(R, T, T1);
				 set(T1, T);

				 stretch(Wx / oldWx, Wy / oldWy, R);
				 times(R, T, T1);
				 set(T1, T);

				 move(Wcx, top, R);
				 times(R, T, T1);
				 set(T1, T);

				 this->Refresh();
			 }
};
}
