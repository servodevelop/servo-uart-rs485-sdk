using BrightJade;
using BrightJade.Serial;
using FashionStar.Servo.Uart;
using FashionStar.Servo.Uart.Protocol;
using System;
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

        public void SyncCommandTest()
        {
            var wrapper = new PacketWrapper<MoveOnAngleModeExByIntervalRequest>();

            var cmd1 = new MoveOnAngleModeExByIntervalRequest();
            var cmd2 = new MoveOnAngleModeExByIntervalRequest();
            var cmd3 = new MoveOnAngleModeExByIntervalRequest();
            cmd1.ID = 1;
            cmd1.Angle = 50;
            cmd2.ID = 2;
            cmd2.Angle = 560;
            cmd3.ID = 3;
            cmd3.Angle = 340;
            wrapper.Add(cmd1);
            wrapper.Add(cmd2);
            wrapper.Add(cmd3);
            var data = wrapper.GetBytes();

            //var symCmd = new SyncCommandRequest();
            //symCmd.ID = cmd1.PacketNumber;
            //symCmd.Length = 11;
            //symCmd.Count = 3;
            //symCmd.Data = data;
            //var bytes = PacketConverterEx.GetBytes(symCmd);

            // Two functions
            //_servoController.SyncCommand(cmd1.PacketNumber, 11, 3, data);
            _servoController.SyncCommand(wrapper);
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

            _servoController.SetOriginPointResponsed += _servoController_SetOriginPointResponsed;

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

        private void _servoController_SetOriginPointResponsed(object sender, DataEventArgs<SetOriginPointResponse> e)
        {
            if (e.Data.Result != 0)
            {
                // TODO: Show dialog for successful setup.
            }
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
    }
}
