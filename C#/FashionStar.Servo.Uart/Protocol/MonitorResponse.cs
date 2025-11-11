using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MonitorResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public ushort Voltage;

        [PacketField]
        public ushort Current;

        [PacketField]
        public ushort Power;

        [PacketField]
        public ushort TemperatureAdc;

        [PacketField]
        public byte ServoStatus;

        [PacketField]
        public int Angle;

        [PacketField]
        public ushort Turns;

        public MonitorResponse() : base(PacketConst.Monitor)
        {

        }
    }
}
