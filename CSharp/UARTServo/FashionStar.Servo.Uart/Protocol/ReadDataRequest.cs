using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ReadDataRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte DataID;

        public ReadDataRequest() : base(PacketConst.ReadData)
        {
        }
    }
}
