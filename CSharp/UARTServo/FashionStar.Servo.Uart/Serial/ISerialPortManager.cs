using System;

namespace BrightJade.Serial
{
    public interface ISerialPortManager : IDisposable
    {
        event EventHandler<SerialDataEventArgs> DataRecieved;
        SerialSettings CurrentSerialSettings { get; set; }
        bool IsOpen { get; }
        string PortName { get; }


        void StartListening();
        void StopListening();
        void Write(byte[] buffer, int offset, int count);
    }
}