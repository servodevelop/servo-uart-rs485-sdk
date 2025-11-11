using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class SyncCommandRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Length;

        [PacketField]
        public byte Count;

        [PacketField(CountBy = "Length*Count")]
        public byte[] Data;

        public SyncCommandRequest() : base(PacketConst.SyncCommand)
        {

        }
    }
}
