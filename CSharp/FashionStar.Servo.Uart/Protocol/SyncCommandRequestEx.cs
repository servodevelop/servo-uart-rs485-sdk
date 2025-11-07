using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class SyncCommandRequestEx : RequestHeaderEx
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Length;

        [PacketField]
        public byte Count;

        [PacketField(CountBy = "Length*Count")]
        public byte[] Data;

        public SyncCommandRequestEx() : base(PacketConst.SyncCommand)
        {

        }
    }
}
