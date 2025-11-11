using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Timers;

namespace FashionStar.Servo.Uart
{
    public class ServoMonitor
    {
        private Timer _timer = new Timer();

        private ServoController _servoController;
        public ServoController ServoController
        {
            get
            {
                return _servoController;
            }
            set
            {
                if (_servoController != null)
                {

                }
                _servoController = value;
                if (_servoController != null)
                {

                }
            }
        }

        public int ScanInterval { get; set; } = 100;

        public int Timeout { get; set; } = 500;

        public ServoMonitor()
        {
        }

        public ServoMonitor(ServoController servoController)
        {
            ServoController = servoController;
        }

        private void SetTimer()
        {
            _timer.AutoReset = true;
            _timer.Interval = ScanInterval;
            _timer.Elapsed += _timer_Elapsed;
        }

        private void _timer_Elapsed(object sender, ElapsedEventArgs e)
        {

        }

        public void StartMonitoring()
        {
            SetTimer();
            _timer.Enabled = true;
        }
    }
}
