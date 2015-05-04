namespace SimApp01CsForm {
partial class SimApp {
	/// <summary>
	/// Required designer variable.
	/// </summary>
	private System.ComponentModel.IContainer components = null;

	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
	protected override void Dispose(bool disposing)
	{
		if (disposing && (components != null))
		{
			components.Dispose();
		}
		base.Dispose(disposing);
	}

	#region Windows Form Designer generated code

	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	private void InitializeComponent()
	{
		this.btnConnect 	= new System.Windows.Forms.Button();
		this.btnEvent1		= new System.Windows.Forms.Button();
		this.btnEvent2		= new System.Windows.Forms.Button();
		this.btnReqData 	= new System.Windows.Forms.Button();
		this.btnDisconnect	= new System.Windows.Forms.Button();
		this.richResponse	= new System.Windows.Forms.RichTextBox();
		this.SuspendLayout();
		//
		// btnConnect
		//
		this.btnConnect.Location	= new System.Drawing.Point(10, 20);
		this.btnConnect.Name		= "btnConnect";
		this.btnConnect.Size		= new System.Drawing.Size(120, 30);
		this.btnConnect.TabIndex	= 0;
		this.btnConnect.Text		= "Connect";
		this.btnConnect.UseVisualStyleBackColor = true;
		this.btnConnect.Click		+= new System.EventHandler(this.btnConnect_Click);
		//
		// btnEvent1
		//
		this.btnEvent1.Location 	= new System.Drawing.Point(10, 60);
		this.btnEvent1.Name	    	= "btnEvent1";
		this.btnEvent1.Size	    	= new System.Drawing.Size(120, 30);
		this.btnEvent1.TabIndex 	= 1;
		this.btnEvent1.Text 		= "do Event 1";
		this.btnEvent1.UseVisualStyleBackColor = true;
		this.btnEvent1.Click		+= new System.EventHandler(this.btnEvent1_Click);
		//
		// btnEvent2
		//
		this.btnEvent2.Location 	= new System.Drawing.Point(10, 100);
		this.btnEvent2.Name 		= "btnEvent2";
		this.btnEvent2.Size 		= new System.Drawing.Size(120, 30);
		this.btnEvent2.TabIndex 	= 2;
		this.btnEvent2.Text 		= "do Event 2";
		this.btnEvent2.UseVisualStyleBackColor = true;
		this.btnEvent2.Click		+= new System.EventHandler(this.btnEvent2_Click);
		//
		// btnReqData
		//
		this.btnReqData.Location	= new System.Drawing.Point(10, 140);
		this.btnReqData.Name		= "btnReqData";
		this.btnReqData.Size		= new System.Drawing.Size(120, 30);
		this.btnReqData.TabIndex	= 3;
		this.btnReqData.Text		= "Request data";
		this.btnReqData.UseVisualStyleBackColor = true;
		this.btnReqData.Click		+= new System.EventHandler(this.btnReqData_Click);
		//
		// btnDisconnect
		//
		this.btnDisconnect.Location	= new System.Drawing.Point(10, 180);
		this.btnDisconnect.Name 	= "btnDisconnect";
		this.btnDisconnect.Size 	= new System.Drawing.Size(120, 30);
		this.btnDisconnect.TabIndex	= 4;
		this.btnDisconnect.Text 	= "Disconnect";
		this.btnDisconnect.UseVisualStyleBackColor = true;
		this.btnDisconnect.Click	+= new System.EventHandler(this.btnDisconnect_Click);
		//
		// richResponse
		//
		this.richResponse.Location	= new System.Drawing.Point(140, 10);
		this.richResponse.Name		= "richResponse";
		this.richResponse.ReadOnly	= true;
		this.richResponse.Size		= new System.Drawing.Size(440, 250);
		this.richResponse.TabIndex	= 5;
		this.richResponse.Text		= "";
		//
		// SimApp
		//
		this.AutoScaleMode	= System.Windows.Forms.AutoScaleMode.None;
		this.ClientSize	= new System.Drawing.Size(594, 272);
		this.Controls.Add(this.richResponse);
		this.Controls.Add(this.btnDisconnect);
		this.Controls.Add(this.btnReqData);
		this.Controls.Add(this.btnEvent2);
		this.Controls.Add(this.btnEvent1);
		this.Controls.Add(this.btnConnect);
		this.Font		= new System.Drawing.Font("Microsoft Sans Serif", 10F,
						System.Drawing.FontStyle.Regular,
						System.Drawing.GraphicsUnit.Point, ((byte)(0)));
		this.FormBorderStyle	= System.Windows.Forms.FormBorderStyle.FixedSingle;
		this.Name		= "SimApp";
		this.Text		= "SimConnect Managed Form";
		this.FormClosed	+= new System.Windows.Forms.FormClosedEventHandler(this.SimApp_FormClosed);
		this.ResumeLayout(false);
	}

	#endregion

	private System.Windows.Forms.Button		btnConnect;
	private System.Windows.Forms.Button		btnEvent1;
	private System.Windows.Forms.Button		btnEvent2;
	private System.Windows.Forms.Button		btnReqData;
	private System.Windows.Forms.Button		btnDisconnect;
	private System.Windows.Forms.RichTextBox	richResponse;
} // ***** end class *****
} // ***** end namespace *****
