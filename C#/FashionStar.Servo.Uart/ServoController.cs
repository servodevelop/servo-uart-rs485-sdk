using BrightJade.Serial;

namespace FashionStar.Servo.Uart
{
    public partial class ServoController
    {
        private ISerialPortManager _serialPortManager;
        public ISerialPortManager SerialPortManager
        {
            get
            {
                return _serialPortManager;
            }
            set
            {
                if (_serialPortManager != null)
                {
                    _serialPortManager.DataRecieved -= SerialPortManagerDataRecieved;
                }
                _serialPortManager = value;
                if (_serialPortManager != null)
                {
                    _serialPortManager.DataRecieved += SerialPortManagerDataRecieved;
                }
            }
        }

        public ServoController()
        {
        }

        public ServoController(ISerialPortManager serialPortManager)
        {
            SerialPortManager = serialPortManager;
        }

        public void StartListening()
        {
            if (SerialPortManager != null)
            {
                SerialPortManager.StartListening();
            }
        }

        public void StopListening()
        {
            if (SerialPortManager != null)
            {
                SerialPortManager.StopListening();
            }
        }

        private void SerialPortManagerDataRecieved(object sender, SerialDataEventArgs e)
        {
            AppendData(e.Data);
        }
    }
}
