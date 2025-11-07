using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ReadDataResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte DataID;

        [PacketField(CountBy = "PacketLength-2")]
        public byte[] Data;

        public object Value;

        public ReadDataResponse() : base(PacketConst.ReadData)
        {
        }
    }
}