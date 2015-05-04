#pragma once

namespace SimApp01Form {

	using namespace System;
	using namespace System::Collections;
	using namespace System::ComponentModel;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Text;
	using namespace System::Windows::Forms;
	using namespace System::Runtime::InteropServices;

	public ref class AppForm : public System::Windows::Forms::Form
	{
	// Note that AppForm is a managed class.
	//
	// own declarations
	private:
		String ^    	output;
		int 	    	response;		// line number
		void	    	setButtons(bool bConnect, bool bGet, bool bDisconnect);
	protected:
		virtual void	DefWndProc(Message% m) override;
	public:
		void	    	displayText(String ^ s);

	// Form Designer declarations
	public:
		AppForm(void);
	protected:
		~AppForm();

	// Form definitions
	private:
		System::Windows::Forms::Button^     	btnConnect;
		System::Windows::Forms::Button^     	btnEvent1;
		System::Windows::Forms::Button^     	btnEvent2;
		System::Windows::Forms::Button^     	btnReqData;
		System::Windows::Forms::Button^     	btnDisconnect;
		System::Windows::Forms::RichTextBox^	richResponse;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnConnect 	= (gcnew System::Windows::Forms::Button());
			this->btnEvent1  	= (gcnew System::Windows::Forms::Button());
			this->btnEvent2  	= (gcnew System::Windows::Forms::Button());
			this->btnReqData 	= (gcnew System::Windows::Forms::Button());
			this->btnDisconnect	= (gcnew System::Windows::Forms::Button());
			this->richResponse	= (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// btnConnect
			// 
			this->btnConnect->Location	= System::Drawing::Point(10, 20);
			this->btnConnect->Name   	= L"btnConnect";
			this->btnConnect->Size   	= System::Drawing::Size(120, 30);
			this->btnConnect->TabIndex	= 0;
			this->btnConnect->Text   	= L"Connect";
			this->btnConnect->UseVisualStyleBackColor	= true;
			this->btnConnect->Click  	+= gcnew System::EventHandler(this, &AppForm::btnConnect_Click);
			// 
			// btnEvent1
			// 
			this->btnEvent1->Location	= System::Drawing::Point(10, 60);
			this->btnEvent1->Name    	= L"btnEvent1";
			this->btnEvent1->Size    	= System::Drawing::Size(120, 30);
			this->btnEvent1->TabIndex	= 1;
			this->btnEvent1->Text    	= L"do Event 1";
			this->btnEvent1->UseVisualStyleBackColor = true;
			this->btnEvent1->Click   	+= gcnew System::EventHandler(this, &AppForm::btnEvent1_Click);
			// 
			// btnEvent2
			// 
			this->btnEvent2->Location	= System::Drawing::Point(10, 100);
			this->btnEvent2->Name    	= L"btnEvent2";
			this->btnEvent2->Size    	= System::Drawing::Size(120, 30);
			this->btnEvent2->TabIndex	= 2;
			this->btnEvent2->Text    	= L"do Event 2";
			this->btnEvent2->UseVisualStyleBackColor = true;
			this->btnEvent2->Click   	+= gcnew System::EventHandler(this, &AppForm::btnEvent2_Click);
			//
			// btnReqData
			//
			this->btnReqData->Location	= System::Drawing::Point(10, 140);
			this->btnReqData->Name   	= L"btnReqData";
			this->btnReqData->Size   	= System::Drawing::Size(120, 30);
			this->btnReqData->TabIndex	= 3;
			this->btnReqData->Text   	= L"Request Data";
			this->btnReqData->UseVisualStyleBackColor = true;
			this->btnReqData->Click  	+= gcnew System::EventHandler(this, &AppForm::btnReqData_Click);
			// 
			// btnDisconnect
			// 
			this->btnDisconnect->Location	= System::Drawing::Point(10, 180);
			this->btnDisconnect->Name    	= L"btnDisconnect";
			this->btnDisconnect->Size    	= System::Drawing::Size(120, 30);
			this->btnDisconnect->TabIndex	= 4;
			this->btnDisconnect->Text    	= L"Disconnect";
			this->btnDisconnect->UseVisualStyleBackColor = true;
			this->btnDisconnect->Click   	+= gcnew System::EventHandler(this, &AppForm::btnDisconnect_Click);
			// 
			// richResponse
			// 
			this->richResponse->Location	= System::Drawing::Point(140, 10);
			this->richResponse->Name    	= L"richResponse";
			this->richResponse->ReadOnly	= true;
			this->richResponse->Size    	= System::Drawing::Size(430, 240);
			this->richResponse->TabIndex	= 5;
			this->richResponse->Text    	= L"";
			//
			// AppForm
			//
			this->AutoScaleMode	= System::Windows::Forms::AutoScaleMode::None;
			this->ClientSize	= System::Drawing::Size(584, 262);
			this->Controls->Add(this->richResponse);
			this->Controls->Add(this->btnDisconnect);
			this->Controls->Add(this->btnReqData);
			this->Controls->Add(this->btnEvent2);
			this->Controls->Add(this->btnEvent1);
			this->Controls->Add(this->btnConnect);
			this->Font			= (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10,
								System::Drawing::FontStyle::Regular,
								System::Drawing::GraphicsUnit::Point,
								static_cast<System::Byte>(0)));
			this->Name	    	= L"AppForm";
			this->Text	    	= L"SimConnect Managed Form";
			this->FormClosed	+= gcnew System::Windows::Forms::FormClosedEventHandler(this, &AppForm::FormClosedEventHandler);
			this->ResumeLayout(false);
		}
#pragma endregion

	// template declaration
	private:
		System::Void btnConnect_Click   	(System::Object^ sender, System::EventArgs^ e);
		System::Void btnEvent1_Click    	(System::Object^ sender, System::EventArgs^ e);
		System::Void btnEvent2_Click    	(System::Object^ sender, System::EventArgs^ e);
		System::Void btnReqData_Click   	(System::Object^ sender, System::EventArgs^ e);
		System::Void btnDisconnect_Click	(System::Object^ sender, System::EventArgs^ e);
		System::Void FormClosedEventHandler	(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
};	// end class		AppForm
}	// end namespace	SimApp01Form
