using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class WriteBatchDataRequest : RequestHeader
    {
        [PacketField(1)]
        public byte ID;

        [PacketField(2, IsCustomType = true)]
        public UserData Data;

        public WriteBatchDataRequest() : base(PacketConst.WriteBatchData)
        {
        }
    }
}
