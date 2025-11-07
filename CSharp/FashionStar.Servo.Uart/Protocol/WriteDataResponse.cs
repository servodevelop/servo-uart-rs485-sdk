using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class WriteDataResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte DataID;

        [PacketField]
        public byte Result;

        public WriteDataResponse() : base(PacketConst.WriteData)
        {
        }
    }
}
