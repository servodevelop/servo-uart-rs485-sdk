using System;
using System.ComponentModel;
using System.IO.Ports;
using System.Reflection;
using System.Threading;

namespace BrightJade.Serial
{
    /// <summary>
    /// Manager for serial port data.
    /// </summary>
    public class SerialPortManager : ISerialPortManager
    {
        public event EventHandler<DataEventArgs<Exception>> ErrorOccured;

        protected virtual void OnErrorOccured(DataEventArgs<Exception> e)
        {
            ErrorOccured?.Invoke(this, e);
        }

        private bool _receiving;
        private Thread _thread;

        public SerialPortManager()
        {
            try
            {
                _receiving = true;

                _thread = new Thread(DoReceive);
                _thread.IsBackground = true;
                _thread.Start();

                // Finding installed serial ports on hardware.
                _currentSerialSettings.PortNameCollection = SerialPort.GetPortNames();
                _currentSerialSettings.PropertyChanged += new PropertyChangedEventHandler(_currentSerialSettings_PropertyChanged);

                // If serial ports is found, we select the first found.
                if (_currentSerialSettings.PortNameCollection.Length > 0)
                    _currentSerialSettings.PortName = _currentSerialSettings.PortNameCollection[0];
            }
            catch (Exception ex)
            {
                OnErrorOccured(new DataEventArgs<Exception>(ex));
            }
        }

        ~SerialPortManager()
        {
            Dispose(false);
        }

        #region Fields

        private SerialPort _serialPort;
        private SerialSettings _currentSerialSettings = new SerialSettings();
        private string _latestRecieved = string.Empty;
        public event EventHandler<SerialDataEventArgs> DataRecieved;

        #endregion

        #region Properties

        public string PortName { get => _currentSerialSettings.PortName; }

        public bool IsOpen
        {
            get { return (_serialPort != null && _serialPort.IsOpen); }
        }
        /// <summary>
        /// Gets or sets the current serial port settings
        /// </summary>
        public SerialSettings CurrentSerialSettings
        {
            get { return _currentSerialSettings; }
            set { _currentSerialSettings = value; }
        }

        #endregion

        #region Event handlers

        private void _currentSerialSettings_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            // if serial port is changed, a new baud query is issued.
            if (e.PropertyName.Equals("PortName"))
            {
                UpdateBaudRateCollection();
            }
        }

        private void DoReceive()
        {
            try
            {
                while (_receiving)
                {
                    if (_serialPort != null && _serialPort.IsOpen)
                    {
                        var bytesToRead = _serialPort.BytesToRead;
                        if (bytesToRead > 0)
                        {
                            var buffer = new Byte[bytesToRead];
                            _serialPort.Read(buffer, 0, bytesToRead);
                            DataRecieved?.Invoke(this, new SerialDataEventArgs(buffer));
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                OnErrorOccured(new DataEventArgs<Exception>(ex));
            }
        }
        #endregion

        #region Methods

        /// <summary>
        /// Connects to a serial port defined through the current settings.
        /// </summary>
        public void StartListening()
        {
            try
            {
                // Closing serial port if it is open.
                if (IsOpen)
                {
                    StopListening();
                }

                // Setting serial port settings.
                _serialPort = new SerialPort(
                    _currentSerialSettings.PortName,
                    _currentSerialSettings.BaudRate,
                    _currentSerialSettings.Parity,
                    _currentSerialSettings.DataBits,
                    _currentSerialSettings.StopBits);

                // Subscribe to event and open serial port for data.
                _serialPort.Open();
            }
            catch (Exception ex)
            {
                OnErrorOccured(new DataEventArgs<Exception>(ex));
            }
        }

        /// <summary>
        /// Closes the serial port.
        /// </summary>
        public void StopListening()
        {
            try
            {
                if (_serialPort.IsOpen)
                {
                    _serialPort.Close();
                }
            }
            catch { }
        }

        public void Write(byte[] buffer, int offset, int count)
        {
            try
            {
                if (IsOpen)
                {
                    _serialPort.Write(buffer, offset, count);
                }
            }
            catch (Exception ex)
            {
                OnErrorOccured(new DataEventArgs<Exception>(ex));
            }
        }

        /// <summary>
        /// Retrieves the current selected device's COMMPROP structure, and extracts the dwSettableBaud property.
        /// </summary>
        private void UpdateBaudRateCollection()
        {
            try
            {
                _serialPort = new SerialPort(_currentSerialSettings.PortName);
                _serialPort.Open();
                object p = _serialPort.BaseStream.GetType().GetField("commProp", BindingFlags.Instance | BindingFlags.NonPublic).GetValue(_serialPort.BaseStream);
                Int32 dwSettableBaud = (Int32)p.GetType().GetField("dwSettableBaud", BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public).GetValue(p);
                p = null;
                System.Threading.Thread.Sleep(100);
                _serialPort.Close();
                _currentSerialSettings.UpdateBaudRateCollection(dwSettableBaud);
            }
            catch (Exception ex)
            {
                OnErrorOccured(new DataEventArgs<Exception>(ex));
            }
        }

        // Call to release serial port.
        public void Dispose()
        {
            Dispose(true);
        }

        // Part of basic design pattern for implementing Dispose.
        protected virtual void Dispose(bool disposing)
        {
            try
            {
                _receiving = false;
                if (_serialPort != null)
                {
                    if (_serialPort.IsOpen)
                        _serialPort.Close();

                    _serialPort.Dispose();
                }
            }
            catch (Exception ex)
            {
                OnErrorOccured(new DataEventArgs<Exception>(ex));
            }
        }

        #endregion

    }
}
