namespace UARTServoDemo
{
    partial class MainForm
    {
        /// <summary>
        /// 設計工具所需的變數。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清除任何使用中的資源。
        /// </summary>
        /// <param name="disposing">如果應該處置受控資源則為 true，否則為 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 設計工具產生的程式碼

        /// <summary>
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
        /// 這個方法的內容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this._btnConnect = new System.Windows.Forms.Button();
            this._btnDisconnect = new System.Windows.Forms.Button();
            this._groupBox = new System.Windows.Forms.GroupBox();
            this._groupBoxLoop = new System.Windows.Forms.GroupBox();
            this._btnResetLoop = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this._lblLoops = new System.Windows.Forms.Label();
            this._btnReadMulitAngle = new System.Windows.Forms.Button();
            this._txtRemainAngle = new System.Windows.Forms.TextBox();
            this._txtLoop = new System.Windows.Forms.TextBox();
            this._groupBoxMultiAngleMode = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this._btnMultiMoveTo = new System.Windows.Forms.Button();
            this._numMultiAngle = new System.Windows.Forms.NumericUpDown();
            this._numIntervalMulti = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this._groupBoxAngleMode = new System.Windows.Forms.GroupBox();
            this._btnMove = new System.Windows.Forms.Button();
            this._numAngle = new System.Windows.Forms.NumericUpDown();
            this._numInterval = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this._groupBoxPosition = new System.Windows.Forms.GroupBox();
            this._txtAngle = new System.Windows.Forms.TextBox();
            this._btnReadAngle = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this._btnStop = new System.Windows.Forms.Button();
            this._numServoID = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this._cboPort = new System.Windows.Forms.ComboBox();
            this._bindingSource = new System.Windows.Forms.BindingSource(this.components);
            this._btnRefresh = new System.Windows.Forms.Button();
            this._cboBaud = new System.Windows.Forms.ComboBox();
            this._groupBox.SuspendLayout();
            this._groupBoxLoop.SuspendLayout();
            this._groupBoxMultiAngleMode.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this._numMultiAngle)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this._numIntervalMulti)).BeginInit();
            this._groupBoxAngleMode.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this._numAngle)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this._numInterval)).BeginInit();
            this._groupBoxPosition.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this._numServoID)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this._bindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // _btnConnect
            // 
            this._btnConnect.Location = new System.Drawing.Point(22, 22);
            this._btnConnect.Name = "_btnConnect";
            this._btnConnect.Size = new System.Drawing.Size(109, 44);
            this._btnConnect.TabIndex = 0;
            this._btnConnect.Text = "Connect";
            this._btnConnect.UseVisualStyleBackColor = true;
            this._btnConnect.Click += new System.EventHandler(this._btnConnect_Click);
            // 
            // _btnDisconnect
            // 
            this._btnDisconnect.Location = new System.Drawing.Point(22, 79);
            this._btnDisconnect.Name = "_btnDisconnect";
            this._btnDisconnect.Size = new System.Drawing.Size(109, 44);
            this._btnDisconnect.TabIndex = 1;
            this._btnDisconnect.Text = "Disonnect";
            this._btnDisconnect.UseVisualStyleBackColor = true;
            this._btnDisconnect.Click += new System.EventHandler(this._btnDisconnect_Click);
            // 
            // _groupBox
            // 
            this._groupBox.Controls.Add(this._groupBoxLoop);
            this._groupBox.Controls.Add(this._groupBoxMultiAngleMode);
            this._groupBox.Controls.Add(this._groupBoxAngleMode);
            this._groupBox.Controls.Add(this._groupBoxPosition);
            this._groupBox.Controls.Add(this._btnStop);
            this._groupBox.Controls.Add(this._numServoID);
            this._groupBox.Controls.Add(this.label4);
            this._groupBox.Location = new System.Drawing.Point(149, 12);
            this._groupBox.Name = "_groupBox";
            this._groupBox.Size = new System.Drawing.Size(639, 531);
            this._groupBox.TabIndex = 3;
            this._groupBox.TabStop = false;
            this._groupBox.Text = "Functions";
            // 
            // _groupBoxLoop
            // 
            this._groupBoxLoop.Controls.Add(this._btnResetLoop);
            this._groupBoxLoop.Controls.Add(this.label8);
            this._groupBoxLoop.Controls.Add(this._lblLoops);
            this._groupBoxLoop.Controls.Add(this._btnReadMulitAngle);
            this._groupBoxLoop.Controls.Add(this._txtRemainAngle);
            this._groupBoxLoop.Controls.Add(this._txtLoop);
            this._groupBoxLoop.Location = new System.Drawing.Point(24, 370);
            this._groupBoxLoop.Name = "_groupBoxLoop";
            this._groupBoxLoop.Size = new System.Drawing.Size(615, 85);
            this._groupBoxLoop.TabIndex = 21;
            this._groupBoxLoop.TabStop = false;
            this._groupBoxLoop.Text = "Loop";
            // 
            // _btnResetLoop
            // 
            this._btnResetLoop.Location = new System.Drawing.Point(413, 31);
            this._btnResetLoop.Name = "_btnResetLoop";
            this._btnResetLoop.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this._btnResetLoop.Size = new System.Drawing.Size(116, 36);
            this._btnResetLoop.TabIndex = 1;
            this._btnResetLoop.Text = "Reset Loop";
            this._btnResetLoop.UseVisualStyleBackColor = true;
            this._btnResetLoop.Click += new System.EventHandler(this._btnResetLoop_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(273, 39);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(12, 18);
            this.label8.TabIndex = 20;
            this.label8.Text = "°";
            // 
            // _lblLoops
            // 
            this._lblLoops.AutoSize = true;
            this._lblLoops.Location = new System.Drawing.Point(114, 43);
            this._lblLoops.Name = "_lblLoops";
            this._lblLoops.Size = new System.Drawing.Size(56, 18);
            this._lblLoops.TabIndex = 19;
            this._lblLoops.Text = "loop(s)";
            // 
            // _btnReadMulitAngle
            // 
            this._btnReadMulitAngle.Location = new System.Drawing.Point(291, 31);
            this._btnReadMulitAngle.Name = "_btnReadMulitAngle";
            this._btnReadMulitAngle.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this._btnReadMulitAngle.Size = new System.Drawing.Size(116, 36);
            this._btnReadMulitAngle.TabIndex = 0;
            this._btnReadMulitAngle.Text = "Update";
            this._btnReadMulitAngle.UseVisualStyleBackColor = true;
            this._btnReadMulitAngle.Click += new System.EventHandler(this._btnReadMulitAngle_Click);
            // 
            // _txtRemainAngle
            // 
            this._txtRemainAngle.BackColor = System.Drawing.SystemColors.Control;
            this._txtRemainAngle.Enabled = false;
            this._txtRemainAngle.Location = new System.Drawing.Point(176, 35);
            this._txtRemainAngle.Name = "_txtRemainAngle";
            this._txtRemainAngle.Size = new System.Drawing.Size(91, 29);
            this._txtRemainAngle.TabIndex = 1;
            this._txtRemainAngle.Text = "Unknown";
            this._txtRemainAngle.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // _txtLoop
            // 
            this._txtLoop.BackColor = System.Drawing.SystemColors.Control;
            this._txtLoop.Enabled = false;
            this._txtLoop.Location = new System.Drawing.Point(12, 35);
            this._txtLoop.Name = "_txtLoop";
            this._txtLoop.Size = new System.Drawing.Size(97, 29);
            this._txtLoop.TabIndex = 0;
            this._txtLoop.Text = "Unknown";
            this._txtLoop.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // _groupBoxMultiAngleMode
            // 
            this._groupBoxMultiAngleMode.Controls.Add(this.label5);
            this._groupBoxMultiAngleMode.Controls.Add(this._btnMultiMoveTo);
            this._groupBoxMultiAngleMode.Controls.Add(this._numMultiAngle);
            this._groupBoxMultiAngleMode.Controls.Add(this._numIntervalMulti);
            this._groupBoxMultiAngleMode.Controls.Add(this.label6);
            this._groupBoxMultiAngleMode.Location = new System.Drawing.Point(12, 267);
            this._groupBoxMultiAngleMode.Name = "_groupBoxMultiAngleMode";
            this._groupBoxMultiAngleMode.Size = new System.Drawing.Size(621, 83);
            this._groupBoxMultiAngleMode.TabIndex = 20;
            this._groupBoxMultiAngleMode.TabStop = false;
            this._groupBoxMultiAngleMode.Text = "Multi-angle Mode";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(327, 41);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(32, 18);
            this.label5.TabIndex = 19;
            this.label5.Text = "ms.";
            // 
            // _btnMultiMoveTo
            // 
            this._btnMultiMoveTo.Location = new System.Drawing.Point(12, 28);
            this._btnMultiMoveTo.Name = "_btnMultiMoveTo";
            this._btnMultiMoveTo.Size = new System.Drawing.Size(109, 44);
            this._btnMultiMoveTo.TabIndex = 12;
            this._btnMultiMoveTo.Text = "Move to";
            this._btnMultiMoveTo.UseVisualStyleBackColor = true;
            this._btnMultiMoveTo.Click += new System.EventHandler(this._btnMultiMoveTo_Click);
            // 
            // _numMultiAngle
            // 
            this._numMultiAngle.Location = new System.Drawing.Point(126, 39);
            this._numMultiAngle.Maximum = new decimal(new int[] {
            368640,
            0,
            0,
            0});
            this._numMultiAngle.Minimum = new decimal(new int[] {
            368640,
            0,
            0,
            -2147483648});
            this._numMultiAngle.Name = "_numMultiAngle";
            this._numMultiAngle.Size = new System.Drawing.Size(84, 29);
            this._numMultiAngle.TabIndex = 13;
            this._numMultiAngle.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // _numIntervalMulti
            // 
            this._numIntervalMulti.Location = new System.Drawing.Point(234, 39);
            this._numIntervalMulti.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this._numIntervalMulti.Name = "_numIntervalMulti";
            this._numIntervalMulti.Size = new System.Drawing.Size(82, 29);
            this._numIntervalMulti.TabIndex = 18;
            this._numIntervalMulti.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this._numIntervalMulti.Value = new decimal(new int[] {
            500,
            0,
            0,
            0});
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(216, 41);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(12, 18);
            this.label6.TabIndex = 15;
            this.label6.Text = "°";
            // 
            // _groupBoxAngleMode
            // 
            this._groupBoxAngleMode.Controls.Add(this._btnMove);
            this._groupBoxAngleMode.Controls.Add(this._numAngle);
            this._groupBoxAngleMode.Controls.Add(this._numInterval);
            this._groupBoxAngleMode.Controls.Add(this.label2);
            this._groupBoxAngleMode.Controls.Add(this.label3);
            this._groupBoxAngleMode.Location = new System.Drawing.Point(9, 67);
            this._groupBoxAngleMode.Name = "_groupBoxAngleMode";
            this._groupBoxAngleMode.Size = new System.Drawing.Size(621, 83);
            this._groupBoxAngleMode.TabIndex = 19;
            this._groupBoxAngleMode.TabStop = false;
            this._groupBoxAngleMode.Text = "Angle Mode";
            // 
            // _btnMove
            // 
            this._btnMove.Location = new System.Drawing.Point(12, 26);
            this._btnMove.Name = "_btnMove";
            this._btnMove.Size = new System.Drawing.Size(109, 44);
            this._btnMove.TabIndex = 4;
            this._btnMove.Text = "Move to";
            this._btnMove.UseVisualStyleBackColor = true;
            this._btnMove.Click += new System.EventHandler(this._btnMove_Click);
            // 
            // _numAngle
            // 
            this._numAngle.Location = new System.Drawing.Point(127, 37);
            this._numAngle.Maximum = new decimal(new int[] {
            130,
            0,
            0,
            0});
            this._numAngle.Minimum = new decimal(new int[] {
            130,
            0,
            0,
            -2147483648});
            this._numAngle.Name = "_numAngle";
            this._numAngle.Size = new System.Drawing.Size(87, 29);
            this._numAngle.TabIndex = 6;
            this._numAngle.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // _numInterval
            // 
            this._numInterval.Location = new System.Drawing.Point(239, 37);
            this._numInterval.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this._numInterval.Name = "_numInterval";
            this._numInterval.Size = new System.Drawing.Size(82, 29);
            this._numInterval.TabIndex = 7;
            this._numInterval.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this._numInterval.Value = new decimal(new int[] {
            500,
            0,
            0,
            0});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(221, 39);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(12, 18);
            this.label2.TabIndex = 8;
            this.label2.Text = "°";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(327, 39);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(32, 18);
            this.label3.TabIndex = 9;
            this.label3.Text = "ms.";
            // 
            // _groupBoxPosition
            // 
            this._groupBoxPosition.Controls.Add(this._txtAngle);
            this._groupBoxPosition.Controls.Add(this._btnReadAngle);
            this._groupBoxPosition.Controls.Add(this.label1);
            this._groupBoxPosition.Location = new System.Drawing.Point(9, 165);
            this._groupBoxPosition.Name = "_groupBoxPosition";
            this._groupBoxPosition.Size = new System.Drawing.Size(624, 84);
            this._groupBoxPosition.TabIndex = 17;
            this._groupBoxPosition.TabStop = false;
            this._groupBoxPosition.Text = "Position";
            // 
            // _txtAngle
            // 
            this._txtAngle.BackColor = System.Drawing.SystemColors.Control;
            this._txtAngle.Enabled = false;
            this._txtAngle.Location = new System.Drawing.Point(127, 38);
            this._txtAngle.Name = "_txtAngle";
            this._txtAngle.Size = new System.Drawing.Size(91, 29);
            this._txtAngle.TabIndex = 6;
            this._txtAngle.Text = "Unknown";
            this._txtAngle.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // _btnReadAngle
            // 
            this._btnReadAngle.Location = new System.Drawing.Point(15, 28);
            this._btnReadAngle.Name = "_btnReadAngle";
            this._btnReadAngle.Size = new System.Drawing.Size(109, 44);
            this._btnReadAngle.TabIndex = 3;
            this._btnReadAngle.Text = "Read Angle";
            this._btnReadAngle.UseVisualStyleBackColor = true;
            this._btnReadAngle.Click += new System.EventHandler(this._btnReadAngle_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(224, 41);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(17, 18);
            this.label1.TabIndex = 5;
            this.label1.Text = "° ";
            // 
            // _btnStop
            // 
            this._btnStop.Location = new System.Drawing.Point(505, 476);
            this._btnStop.Name = "_btnStop";
            this._btnStop.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this._btnStop.Size = new System.Drawing.Size(109, 36);
            this._btnStop.TabIndex = 16;
            this._btnStop.Text = "Stop";
            this._btnStop.UseVisualStyleBackColor = true;
            this._btnStop.Click += new System.EventHandler(this._btnStop_Click);
            // 
            // _numServoID
            // 
            this._numServoID.Location = new System.Drawing.Point(87, 21);
            this._numServoID.Maximum = new decimal(new int[] {
            254,
            0,
            0,
            0});
            this._numServoID.Name = "_numServoID";
            this._numServoID.Size = new System.Drawing.Size(69, 29);
            this._numServoID.TabIndex = 11;
            this._numServoID.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 25);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(75, 18);
            this.label4.TabIndex = 10;
            this.label4.Text = "Servo ID:";
            // 
            // _cboPort
            // 
            this._cboPort.DataBindings.Add(new System.Windows.Forms.Binding("Text", this._bindingSource, "PortName", true));
            this._cboPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._cboPort.FormattingEnabled = true;
            this._cboPort.Location = new System.Drawing.Point(22, 147);
            this._cboPort.Name = "_cboPort";
            this._cboPort.Size = new System.Drawing.Size(104, 26);
            this._cboPort.TabIndex = 4;
            // 
            // _bindingSource
            // 
            this._bindingSource.DataSource = typeof(BrightJade.Serial.SerialSettings);
            // 
            // _btnRefresh
            // 
            this._btnRefresh.Location = new System.Drawing.Point(17, 382);
            this._btnRefresh.Name = "_btnRefresh";
            this._btnRefresh.Size = new System.Drawing.Size(109, 44);
            this._btnRefresh.TabIndex = 12;
            this._btnRefresh.Text = "Refresh";
            this._btnRefresh.UseVisualStyleBackColor = true;
            this._btnRefresh.Click += new System.EventHandler(this._btnRefresh_Click);
            // 
            // _cboBaud
            // 
            this._cboBaud.DataBindings.Add(new System.Windows.Forms.Binding("Text", this._bindingSource, "BaudRate", true));
            this._cboBaud.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._cboBaud.FormattingEnabled = true;
            this._cboBaud.Location = new System.Drawing.Point(22, 199);
            this._cboBaud.Name = "_cboBaud";
            this._cboBaud.Size = new System.Drawing.Size(104, 26);
            this._cboBaud.TabIndex = 13;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 555);
            this.Controls.Add(this._cboBaud);
            this.Controls.Add(this._btnRefresh);
            this.Controls.Add(this._cboPort);
            this.Controls.Add(this._groupBox);
            this.Controls.Add(this._btnDisconnect);
            this.Controls.Add(this._btnConnect);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "MainForm";
            this.Text = "UART Servo Demo";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this._groupBox.ResumeLayout(false);
            this._groupBox.PerformLayout();
            this._groupBoxLoop.ResumeLayout(false);
            this._groupBoxLoop.PerformLayout();
            this._groupBoxMultiAngleMode.ResumeLayout(false);
            this._groupBoxMultiAngleMode.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this._numMultiAngle)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this._numIntervalMulti)).EndInit();
            this._groupBoxAngleMode.ResumeLayout(false);
            this._groupBoxAngleMode.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this._numAngle)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this._numInterval)).EndInit();
            this._groupBoxPosition.ResumeLayout(false);
            this._groupBoxPosition.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this._numServoID)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this._bindingSource)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button _btnConnect;
        private System.Windows.Forms.Button _btnDisconnect;
        private System.Windows.Forms.GroupBox _groupBox;
        private System.Windows.Forms.Button _btnReadAngle;
        private System.Windows.Forms.Button _btnMove;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown _numInterval;
        private System.Windows.Forms.NumericUpDown _numAngle;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown _numServoID;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox _cboPort;
        private System.Windows.Forms.Button _btnRefresh;
        private System.Windows.Forms.ComboBox _cboBaud;
        private System.Windows.Forms.BindingSource _bindingSource;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown _numMultiAngle;
        private System.Windows.Forms.Button _btnMultiMoveTo;
        private System.Windows.Forms.Button _btnStop;
        private System.Windows.Forms.GroupBox _groupBoxMultiAngleMode;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown _numIntervalMulti;
        private System.Windows.Forms.GroupBox _groupBoxAngleMode;
        private System.Windows.Forms.GroupBox _groupBoxPosition;
        private System.Windows.Forms.GroupBox _groupBoxLoop;
        private System.Windows.Forms.Button _btnResetLoop;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label _lblLoops;
        private System.Windows.Forms.Button _btnReadMulitAngle;
        private System.Windows.Forms.TextBox _txtRemainAngle;
        private System.Windows.Forms.TextBox _txtLoop;
        private System.Windows.Forms.TextBox _txtAngle;
    }
}

