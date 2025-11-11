using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class WriteBatchDataResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public WriteBatchDataResponse() : base(PacketConst.WriteBatchData)
        {
        }
    }
}
