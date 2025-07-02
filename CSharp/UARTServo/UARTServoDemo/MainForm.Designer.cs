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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this._btnConnect = new System.Windows.Forms.Button();
            this._btnDisconnect = new System.Windows.Forms.Button();
            this._groupBox = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.button6 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.label12 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button1 = new System.Windows.Forms.Button();
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
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this._groupBox.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
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
            this._btnConnect.Location = new System.Drawing.Point(15, 15);
            this._btnConnect.Margin = new System.Windows.Forms.Padding(2);
            this._btnConnect.Name = "_btnConnect";
            this._btnConnect.Size = new System.Drawing.Size(73, 29);
            this._btnConnect.TabIndex = 0;
            this._btnConnect.Text = "Connect";
            this._btnConnect.UseVisualStyleBackColor = true;
            this._btnConnect.Click += new System.EventHandler(this._btnConnect_Click);
            // 
            // _btnDisconnect
            // 
            this._btnDisconnect.Location = new System.Drawing.Point(15, 53);
            this._btnDisconnect.Margin = new System.Windows.Forms.Padding(2);
            this._btnDisconnect.Name = "_btnDisconnect";
            this._btnDisconnect.Size = new System.Drawing.Size(73, 29);
            this._btnDisconnect.TabIndex = 1;
            this._btnDisconnect.Text = "Disonnect";
            this._btnDisconnect.UseVisualStyleBackColor = true;
            this._btnDisconnect.Click += new System.EventHandler(this._btnDisconnect_Click);
            // 
            // _groupBox
            // 
            this._groupBox.Controls.Add(this.groupBox2);
            this._groupBox.Controls.Add(this.groupBox1);
            this._groupBox.Controls.Add(this._groupBoxLoop);
            this._groupBox.Controls.Add(this._groupBoxMultiAngleMode);
            this._groupBox.Controls.Add(this._groupBoxAngleMode);
            this._groupBox.Controls.Add(this._groupBoxPosition);
            this._groupBox.Controls.Add(this._btnStop);
            this._groupBox.Controls.Add(this._numServoID);
            this._groupBox.Controls.Add(this.label4);
            this._groupBox.Location = new System.Drawing.Point(99, 8);
            this._groupBox.Margin = new System.Windows.Forms.Padding(2);
            this._groupBox.Name = "_groupBox";
            this._groupBox.Padding = new System.Windows.Forms.Padding(2);
            this._groupBox.Size = new System.Drawing.Size(750, 354);
            this._groupBox.TabIndex = 3;
            this._groupBox.TabStop = false;
            this._groupBox.Text = "Functions";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label14);
            this.groupBox2.Controls.Add(this.label13);
            this.groupBox2.Controls.Add(this.label10);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.button6);
            this.groupBox2.Controls.Add(this.button5);
            this.groupBox2.Controls.Add(this.button4);
            this.groupBox2.Controls.Add(this.button3);
            this.groupBox2.Controls.Add(this.button2);
            this.groupBox2.Controls.Add(this.label12);
            this.groupBox2.Controls.Add(this.label11);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Controls.Add(this.textBox5);
            this.groupBox2.Controls.Add(this.textBox4);
            this.groupBox2.Controls.Add(this.textBox3);
            this.groupBox2.Controls.Add(this.textBox2);
            this.groupBox2.Controls.Add(this.textBox1);
            this.groupBox2.Location = new System.Drawing.Point(447, 110);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(283, 194);
            this.groupBox2.TabIndex = 23;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Read Data";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(215, 126);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(17, 12);
            this.label14.TabIndex = 30;
            this.label14.Text = "℃";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(215, 94);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(17, 12);
            this.label13.TabIndex = 29;
            this.label13.Text = "mW";
            this.label13.Click += new System.EventHandler(this.label13_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(215, 62);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(17, 12);
            this.label10.TabIndex = 28;
            this.label10.Text = "mA";
            this.label10.Click += new System.EventHandler(this.label10_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(215, 30);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(17, 12);
            this.label7.TabIndex = 27;
            this.label7.Text = "mV";
            this.label7.Click += new System.EventHandler(this.label7_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(15, 154);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(87, 23);
            this.button6.TabIndex = 24;
            this.button6.Text = "Status";
            this.toolTip1.SetToolTip(this.button6, resources.GetString("button6.ToolTip"));
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(15, 122);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(87, 23);
            this.button5.TabIndex = 26;
            this.button5.Text = "Temperature";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(15, 90);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(87, 23);
            this.button4.TabIndex = 25;
            this.button4.Text = "Power";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(15, 58);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(87, 23);
            this.button3.TabIndex = 24;
            this.button3.Text = "Current";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(15, 26);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(87, 23);
            this.button2.TabIndex = 24;
            this.button2.Text = "Voltage";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click_1);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(19, 139);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(0, 12);
            this.label12.TabIndex = 10;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(19, 114);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(0, 12);
            this.label11.TabIndex = 9;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(18, 57);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(0, 12);
            this.label9.TabIndex = 7;
            // 
            // textBox5
            // 
            this.textBox5.Enabled = false;
            this.textBox5.Location = new System.Drawing.Point(108, 154);
            this.textBox5.Name = "textBox5";
            this.textBox5.ReadOnly = true;
            this.textBox5.Size = new System.Drawing.Size(100, 21);
            this.textBox5.TabIndex = 5;
            // 
            // textBox4
            // 
            this.textBox4.Enabled = false;
            this.textBox4.Location = new System.Drawing.Point(108, 122);
            this.textBox4.Name = "textBox4";
            this.textBox4.ReadOnly = true;
            this.textBox4.Size = new System.Drawing.Size(100, 21);
            this.textBox4.TabIndex = 4;
            // 
            // textBox3
            // 
            this.textBox3.Enabled = false;
            this.textBox3.Location = new System.Drawing.Point(108, 90);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size(100, 21);
            this.textBox3.TabIndex = 3;
            // 
            // textBox2
            // 
            this.textBox2.Enabled = false;
            this.textBox2.Location = new System.Drawing.Point(108, 58);
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(100, 21);
            this.textBox2.TabIndex = 2;
            // 
            // textBox1
            // 
            this.textBox1.Enabled = false;
            this.textBox1.Location = new System.Drawing.Point(108, 26);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(100, 21);
            this.textBox1.TabIndex = 1;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Location = new System.Drawing.Point(447, 45);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(283, 55);
            this.groupBox1.TabIndex = 22;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Set Origin Point";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(15, 20);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(107, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Stop And Set";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // _groupBoxLoop
            // 
            this._groupBoxLoop.Controls.Add(this._btnResetLoop);
            this._groupBoxLoop.Controls.Add(this.label8);
            this._groupBoxLoop.Controls.Add(this._lblLoops);
            this._groupBoxLoop.Controls.Add(this._btnReadMulitAngle);
            this._groupBoxLoop.Controls.Add(this._txtRemainAngle);
            this._groupBoxLoop.Controls.Add(this._txtLoop);
            this._groupBoxLoop.Location = new System.Drawing.Point(16, 247);
            this._groupBoxLoop.Margin = new System.Windows.Forms.Padding(2);
            this._groupBoxLoop.Name = "_groupBoxLoop";
            this._groupBoxLoop.Padding = new System.Windows.Forms.Padding(2);
            this._groupBoxLoop.Size = new System.Drawing.Size(410, 57);
            this._groupBoxLoop.TabIndex = 21;
            this._groupBoxLoop.TabStop = false;
            this._groupBoxLoop.Text = "Loop";
            // 
            // _btnResetLoop
            // 
            this._btnResetLoop.Location = new System.Drawing.Point(275, 21);
            this._btnResetLoop.Margin = new System.Windows.Forms.Padding(2);
            this._btnResetLoop.Name = "_btnResetLoop";
            this._btnResetLoop.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this._btnResetLoop.Size = new System.Drawing.Size(77, 24);
            this._btnResetLoop.TabIndex = 1;
            this._btnResetLoop.Text = "Reset Loop";
            this._btnResetLoop.UseVisualStyleBackColor = true;
            this._btnResetLoop.Click += new System.EventHandler(this._btnResetLoop_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(182, 26);
            this.label8.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(17, 12);
            this.label8.TabIndex = 20;
            this.label8.Text = "°";
            // 
            // _lblLoops
            // 
            this._lblLoops.AutoSize = true;
            this._lblLoops.Location = new System.Drawing.Point(76, 29);
            this._lblLoops.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this._lblLoops.Name = "_lblLoops";
            this._lblLoops.Size = new System.Drawing.Size(47, 12);
            this._lblLoops.TabIndex = 19;
            this._lblLoops.Text = "loop(s)";
            // 
            // _btnReadMulitAngle
            // 
            this._btnReadMulitAngle.Location = new System.Drawing.Point(194, 21);
            this._btnReadMulitAngle.Margin = new System.Windows.Forms.Padding(2);
            this._btnReadMulitAngle.Name = "_btnReadMulitAngle";
            this._btnReadMulitAngle.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this._btnReadMulitAngle.Size = new System.Drawing.Size(77, 24);
            this._btnReadMulitAngle.TabIndex = 0;
            this._btnReadMulitAngle.Text = "Update";
            this._btnReadMulitAngle.UseVisualStyleBackColor = true;
            this._btnReadMulitAngle.Click += new System.EventHandler(this._btnReadMulitAngle_Click);
            // 
            // _txtRemainAngle
            // 
            this._txtRemainAngle.BackColor = System.Drawing.SystemColors.Control;
            this._txtRemainAngle.Enabled = false;
            this._txtRemainAngle.Location = new System.Drawing.Point(117, 23);
            this._txtRemainAngle.Margin = new System.Windows.Forms.Padding(2);
            this._txtRemainAngle.Name = "_txtRemainAngle";
            this._txtRemainAngle.Size = new System.Drawing.Size(62, 21);
            this._txtRemainAngle.TabIndex = 1;
            this._txtRemainAngle.Text = "Unknown";
            this._txtRemainAngle.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // _txtLoop
            // 
            this._txtLoop.BackColor = System.Drawing.SystemColors.Control;
            this._txtLoop.Enabled = false;
            this._txtLoop.Location = new System.Drawing.Point(8, 23);
            this._txtLoop.Margin = new System.Windows.Forms.Padding(2);
            this._txtLoop.Name = "_txtLoop";
            this._txtLoop.Size = new System.Drawing.Size(66, 21);
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
            this._groupBoxMultiAngleMode.Location = new System.Drawing.Point(8, 178);
            this._groupBoxMultiAngleMode.Margin = new System.Windows.Forms.Padding(2);
            this._groupBoxMultiAngleMode.Name = "_groupBoxMultiAngleMode";
            this._groupBoxMultiAngleMode.Padding = new System.Windows.Forms.Padding(2);
            this._groupBoxMultiAngleMode.Size = new System.Drawing.Size(414, 55);
            this._groupBoxMultiAngleMode.TabIndex = 20;
            this._groupBoxMultiAngleMode.TabStop = false;
            this._groupBoxMultiAngleMode.Text = "Multi-angle Mode";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(218, 27);
            this.label5.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(23, 12);
            this.label5.TabIndex = 19;
            this.label5.Text = "ms.";
            // 
            // _btnMultiMoveTo
            // 
            this._btnMultiMoveTo.Location = new System.Drawing.Point(8, 19);
            this._btnMultiMoveTo.Margin = new System.Windows.Forms.Padding(2);
            this._btnMultiMoveTo.Name = "_btnMultiMoveTo";
            this._btnMultiMoveTo.Size = new System.Drawing.Size(73, 29);
            this._btnMultiMoveTo.TabIndex = 12;
            this._btnMultiMoveTo.Text = "Move to";
            this._btnMultiMoveTo.UseVisualStyleBackColor = true;
            this._btnMultiMoveTo.Click += new System.EventHandler(this._btnMultiMoveTo_Click);
            // 
            // _numMultiAngle
            // 
            this._numMultiAngle.Location = new System.Drawing.Point(84, 26);
            this._numMultiAngle.Margin = new System.Windows.Forms.Padding(2);
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
            this._numMultiAngle.Size = new System.Drawing.Size(56, 21);
            this._numMultiAngle.TabIndex = 13;
            this._numMultiAngle.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // _numIntervalMulti
            // 
            this._numIntervalMulti.Location = new System.Drawing.Point(156, 26);
            this._numIntervalMulti.Margin = new System.Windows.Forms.Padding(2);
            this._numIntervalMulti.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this._numIntervalMulti.Name = "_numIntervalMulti";
            this._numIntervalMulti.Size = new System.Drawing.Size(55, 21);
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
            this.label6.Location = new System.Drawing.Point(144, 27);
            this.label6.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(17, 12);
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
            this._groupBoxAngleMode.Location = new System.Drawing.Point(6, 45);
            this._groupBoxAngleMode.Margin = new System.Windows.Forms.Padding(2);
            this._groupBoxAngleMode.Name = "_groupBoxAngleMode";
            this._groupBoxAngleMode.Padding = new System.Windows.Forms.Padding(2);
            this._groupBoxAngleMode.Size = new System.Drawing.Size(414, 55);
            this._groupBoxAngleMode.TabIndex = 19;
            this._groupBoxAngleMode.TabStop = false;
            this._groupBoxAngleMode.Text = "Angle Mode";
            // 
            // _btnMove
            // 
            this._btnMove.Location = new System.Drawing.Point(8, 17);
            this._btnMove.Margin = new System.Windows.Forms.Padding(2);
            this._btnMove.Name = "_btnMove";
            this._btnMove.Size = new System.Drawing.Size(73, 29);
            this._btnMove.TabIndex = 4;
            this._btnMove.Text = "Move to";
            this._btnMove.UseVisualStyleBackColor = true;
            this._btnMove.Click += new System.EventHandler(this._btnMove_Click);
            // 
            // _numAngle
            // 
            this._numAngle.Location = new System.Drawing.Point(85, 25);
            this._numAngle.Margin = new System.Windows.Forms.Padding(2);
            this._numAngle.Maximum = new decimal(new int[] {
            180,
            0,
            0,
            0});
            this._numAngle.Minimum = new decimal(new int[] {
            180,
            0,
            0,
            -2147483648});
            this._numAngle.Name = "_numAngle";
            this._numAngle.Size = new System.Drawing.Size(58, 21);
            this._numAngle.TabIndex = 6;
            this._numAngle.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // _numInterval
            // 
            this._numInterval.Location = new System.Drawing.Point(159, 25);
            this._numInterval.Margin = new System.Windows.Forms.Padding(2);
            this._numInterval.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this._numInterval.Name = "_numInterval";
            this._numInterval.Size = new System.Drawing.Size(55, 21);
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
            this.label2.Location = new System.Drawing.Point(147, 26);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(17, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "°";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(218, 26);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(23, 12);
            this.label3.TabIndex = 9;
            this.label3.Text = "ms.";
            // 
            // _groupBoxPosition
            // 
            this._groupBoxPosition.Controls.Add(this._txtAngle);
            this._groupBoxPosition.Controls.Add(this._btnReadAngle);
            this._groupBoxPosition.Controls.Add(this.label1);
            this._groupBoxPosition.Location = new System.Drawing.Point(6, 110);
            this._groupBoxPosition.Margin = new System.Windows.Forms.Padding(2);
            this._groupBoxPosition.Name = "_groupBoxPosition";
            this._groupBoxPosition.Padding = new System.Windows.Forms.Padding(2);
            this._groupBoxPosition.Size = new System.Drawing.Size(416, 56);
            this._groupBoxPosition.TabIndex = 17;
            this._groupBoxPosition.TabStop = false;
            this._groupBoxPosition.Text = "Position";
            // 
            // _txtAngle
            // 
            this._txtAngle.BackColor = System.Drawing.SystemColors.Control;
            this._txtAngle.Enabled = false;
            this._txtAngle.Location = new System.Drawing.Point(85, 25);
            this._txtAngle.Margin = new System.Windows.Forms.Padding(2);
            this._txtAngle.Name = "_txtAngle";
            this._txtAngle.Size = new System.Drawing.Size(62, 21);
            this._txtAngle.TabIndex = 6;
            this._txtAngle.Text = "Unknown";
            this._txtAngle.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // _btnReadAngle
            // 
            this._btnReadAngle.Location = new System.Drawing.Point(10, 19);
            this._btnReadAngle.Margin = new System.Windows.Forms.Padding(2);
            this._btnReadAngle.Name = "_btnReadAngle";
            this._btnReadAngle.Size = new System.Drawing.Size(73, 29);
            this._btnReadAngle.TabIndex = 3;
            this._btnReadAngle.Text = "Read Angle";
            this._btnReadAngle.UseVisualStyleBackColor = true;
            this._btnReadAngle.Click += new System.EventHandler(this._btnReadAngle_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(149, 27);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(23, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "° ";
            // 
            // _btnStop
            // 
            this._btnStop.Location = new System.Drawing.Point(353, 308);
            this._btnStop.Margin = new System.Windows.Forms.Padding(2);
            this._btnStop.Name = "_btnStop";
            this._btnStop.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this._btnStop.Size = new System.Drawing.Size(73, 24);
            this._btnStop.TabIndex = 16;
            this._btnStop.Text = "Stop";
            this._btnStop.UseVisualStyleBackColor = true;
            this._btnStop.Click += new System.EventHandler(this._btnStop_Click);
            // 
            // _numServoID
            // 
            this._numServoID.Location = new System.Drawing.Point(58, 14);
            this._numServoID.Margin = new System.Windows.Forms.Padding(2);
            this._numServoID.Maximum = new decimal(new int[] {
            254,
            0,
            0,
            0});
            this._numServoID.Name = "_numServoID";
            this._numServoID.Size = new System.Drawing.Size(46, 21);
            this._numServoID.TabIndex = 11;
            this._numServoID.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(4, 17);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(59, 12);
            this.label4.TabIndex = 10;
            this.label4.Text = "Servo ID:";
            // 
            // _cboPort
            // 
            this._cboPort.DataBindings.Add(new System.Windows.Forms.Binding("Text", this._bindingSource, "PortName", true));
            this._cboPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._cboPort.FormattingEnabled = true;
            this._cboPort.Location = new System.Drawing.Point(15, 98);
            this._cboPort.Margin = new System.Windows.Forms.Padding(2);
            this._cboPort.Name = "_cboPort";
            this._cboPort.Size = new System.Drawing.Size(71, 20);
            this._cboPort.TabIndex = 4;
            // 
            // _bindingSource
            // 
            this._bindingSource.DataSource = typeof(BrightJade.Serial.SerialSettings);
            // 
            // _btnRefresh
            // 
            this._btnRefresh.Location = new System.Drawing.Point(11, 255);
            this._btnRefresh.Margin = new System.Windows.Forms.Padding(2);
            this._btnRefresh.Name = "_btnRefresh";
            this._btnRefresh.Size = new System.Drawing.Size(73, 29);
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
            this._cboBaud.Location = new System.Drawing.Point(15, 133);
            this._cboBaud.Margin = new System.Windows.Forms.Padding(2);
            this._cboBaud.Name = "_cboBaud";
            this._cboBaud.Size = new System.Drawing.Size(71, 20);
            this._cboBaud.TabIndex = 13;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(863, 374);
            this.Controls.Add(this._cboBaud);
            this.Controls.Add(this._btnRefresh);
            this.Controls.Add(this._cboPort);
            this.Controls.Add(this._groupBox);
            this.Controls.Add(this._btnDisconnect);
            this.Controls.Add(this._btnConnect);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "MainForm";
            this.Text = "UART Servo Demo";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this._groupBox.ResumeLayout(false);
            this._groupBox.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
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
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ToolTip toolTip1;
    }
}

