using BrightJade;
using BrightJade.Serial;
using FashionStar.Servo.Uart;
using FashionStar.Servo.Uart.Protocol;
using System;
using System.Drawing.Text;
using System.Windows.Forms;

namespace UARTServoDemo
{
    public partial class MainForm : Form
    {
        private SerialPortManager _serialPortManager;
        private ServoController _servoController;

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            ConnctPreparation();
        }

        private void ConnctPreparation()
        {
            // Recover UI to disconnect status.
            ConnectSwitch(false);

            // When call ConnctPreparation() again, we need it.
            if (_servoController != null)
            {
                _servoController.StopListening();
                _servoController.ReadAngleResponsed -= OnReadAngleResponsed;
                _servoController.ReadMultiTurnAngleResponsed -= OnReadMultiTurnAngleResponsed;
                _serialPortManager.ErrorOccured -= OnErrorOccured;
            }

            _serialPortManager = new SerialPortManager();
            _servoController = new ServoController(_serialPortManager);

            _servoController.ReadAngleResponsed += OnReadAngleResponsed;
            _servoController.ReadMultiTurnAngleResponsed += OnReadMultiTurnAngleResponsed;
            _serialPortManager.ErrorOccured += OnErrorOccured;
            _servoController.ReadDataResponsed += OnReadDataResponsed;
            SerialSettings settings = _serialPortManager.CurrentSerialSettings;

            // If PortNameCollection != null, PortName == PortNameCollection[0]
            bool isSerialPortOK = !string.IsNullOrEmpty(settings.PortName);

            if (isSerialPortOK)
            {
                _bindingSource.DataSource = settings;
                _cboPort.DataSource = settings.PortNameCollection;
                _cboBaud.DataSource = settings.BaudRateCollection;

                // Set default value.
                settings.BaudRate = 115200;
            }

            SerialPortReadySwtich(isSerialPortOK);
        }

        private void OnErrorOccured(object sender, DataEventArgs<Exception> e)
        {
            if (InvokeRequired)
            {
                BeginInvoke((MethodInvoker)delegate { OnErrorOccured(sender, e); });
            }
            else
            {
                MessageBox.Show(e.Data.Message, "Serial Port Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                ConnectSwitch(false);
                SerialPortReadySwtich(false);
            }
        }

        private void OnReadMultiTurnAngleResponsed(object sender, DataEventArgs<ReadMultiTurnAngleResponse> e)
        {
            if (InvokeRequired)
            {
                BeginInvoke((MethodInvoker)delegate { OnReadMultiTurnAngleResponsed(sender, e); });
            }
            else
            {
                double angle = e.Data.Angle / 10.0;
                _txtRemainAngle.Text = angle.ToString();
                _txtLoop.Text = e.Data.Turns.ToString();
            }
        }

        private void OnReadAngleResponsed(object sender, DataEventArgs<ReadAngleResponse> e)
        {
            if (InvokeRequired)
            {
                BeginInvoke((MethodInvoker)delegate { OnReadAngleResponsed(sender, e); });
            }
            else
            {
                double angle = e.Data.Angle / 10.0;
                _txtAngle.Text = angle.ToString();
            }
        }

        private void _btnConnect_Click(object sender, EventArgs e)
        {
            ConnectSwitch(true);
            _servoController.StartListening();
        }

        private void _btnDisconnect_Click(object sender, EventArgs e)
        {
            ConnectSwitch(false);
            _servoController.StopListening();
        }

        private void _btnReadAngle_Click(object sender, EventArgs e)
        {
            _servoController.ReadAngle((byte)_numServoID.Value);
        }

        private void _btnReadMulitAngle_Click(object sender, EventArgs e)
        {
            _servoController.ReadMultiTurnAngle((byte)_numServoID.Value);
        }

        private void _btnMove_Click(object sender, EventArgs e)
        {
            _servoController.MoveOnAngleMode((byte)_numServoID.Value, (short)(_numAngle.Value * 10), (ushort)_numInterval.Value);
        }

        private void _btnMultiMoveTo_Click(object sender, EventArgs e)
        {
            _servoController.MoveOnMultiTurnAngleMode((byte)_numServoID.Value, (int)(_numMultiAngle.Value * 10), (uint)_numIntervalMulti.Value);
        }

        private void _btnStop_Click(object sender, EventArgs e)
        {
            _servoController.Stop((byte)_numServoID.Value);
        }

        private void _btnRefresh_Click(object sender, EventArgs e)
        {
            ConnctPreparation();
        }

        private void ConnectSwitch(bool isConnect)
        {
            _groupBox.Enabled = isConnect;
            _btnConnect.Enabled = !isConnect;
            _btnDisconnect.Enabled = isConnect;
            _cboPort.Enabled = !isConnect;
            _cboBaud.Enabled = !isConnect;
        }

        private void SerialPortReadySwtich(bool isReady)
        {
            _btnConnect.Enabled = isReady;
            _cboPort.Enabled = isReady;
            _cboBaud.Enabled = isReady;
        }

        private void _btnResetLoop_Click(object sender, EventArgs e)
        {
            _servoController.ResetMultiTurnAngle((byte)_numServoID.Value);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            _servoController.Stop((byte)_numServoID.Value);
            _servoController.SetOriginPoint((byte)_numServoID.Value);
            _servoController.ReadAngle((byte)_numServoID.Value);
        }
        private void OnReadDataResponsed(object sender, DataEventArgs<ReadDataResponse> e)
        {
            if (InvokeRequired)
            {
                BeginInvoke((MethodInvoker)delegate { OnReadDataResponsed(sender, e); });
            }
            else
            {
                BasicData data = new BasicData();
                
                if (e.Data.DataID == DataConst.Voltage)
                {
                    data.Voltage = (ushort)e.Data.Value;
                    textBox1.Text = data.Voltage.ToString();
                }
                else if(e.Data.DataID == DataConst.Current)
                {
                    data.Current = (ushort)e.Data.Value;
                    textBox2.Text = data.Current.ToString();
                }
                else if (e.Data.DataID == DataConst.Power)
                {
                    data.Power = (ushort)e.Data.Value;
                    textBox3.Text = data.Power.ToString();
                }
                else if (e.Data.DataID == DataConst.Temperature)
                {
                    data.TemperatureAdc = (ushort)e.Data.Value;
                    textBox4.Text = data.Temperature.ToString();
                }
                else if (e.Data.DataID == DataConst.Status)
                {
                    data.Status = (byte)e.Data.Value;
                    textBox5.Text = Convert.ToString(data.Status, 2).PadLeft(8,'0');
                }

            }
        }
        private void button6_Click(object sender, EventArgs e)
        {
            _servoController.ReadData((byte)_numServoID.Value, DataConst.Status);
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            _servoController.ReadData((byte)_numServoID.Value, DataConst.Voltage);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            _servoController.ReadData((byte)_numServoID.Value, DataConst.Current);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            _servoController.ReadData((byte)_numServoID.Value, DataConst.Power);
        }
        private void button5_Click(object sender, EventArgs e)
        {
            _servoController.ReadData((byte)_numServoID.Value, DataConst.Temperature);
        }

        private void label13_Click(object sender, EventArgs e)
        {

        }

        private void label10_Click(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }
    }
}
