using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ReadBatchDataResponse : ResponseHeader
    {
        [PacketField(1)]
        public byte ID;

        [PacketField(2, IsCustomType = true)]
        public UserData Data;

        public ReadBatchDataResponse() : base(PacketConst.ReadBatchData)
        {
        }
    }
}
