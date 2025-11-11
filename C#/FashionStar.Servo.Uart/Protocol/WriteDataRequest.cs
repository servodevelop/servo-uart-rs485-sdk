using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class WriteDataRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte DataID;

        [PacketField(CountBy = "PacketLength-2")]
        public byte[] Data;

        public WriteDataRequest() : base(PacketConst.WriteData)
        {
        }
    }
}
