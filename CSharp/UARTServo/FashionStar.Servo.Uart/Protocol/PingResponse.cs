using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class PingResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        public PingResponse() : base(PacketConst.Ping)
        {
        }
    }
}
