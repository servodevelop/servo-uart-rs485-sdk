using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class PingRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        public PingRequest() : base(PacketConst.Ping)
        {
        }
    }
}
