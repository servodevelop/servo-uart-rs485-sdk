using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MonitorRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        public MonitorRequest() : base(PacketConst.Monitor)
        {
        }
    }
}
