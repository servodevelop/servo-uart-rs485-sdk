using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ReadBatchDataRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        public ReadBatchDataRequest() : base(PacketConst.ReadBatchData)
        {
        }
    }
}
