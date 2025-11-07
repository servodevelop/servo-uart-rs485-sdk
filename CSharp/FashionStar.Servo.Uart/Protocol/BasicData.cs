using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class BasicData
    {
        public ushort Voltage;
        public ushort Current;
        public ushort Power;
        public ushort TemperatureAdc;
        public ushort Temperature => (ushort)TemperatureConverter.AdcToDegree(TemperatureAdc);
        public byte Status;
        public ushort Model;
        public ushort Version;
        public uint SerialNumber;
    }
}
