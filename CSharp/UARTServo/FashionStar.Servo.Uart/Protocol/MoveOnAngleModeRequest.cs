using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnAngleModeRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public short Angle;

        [PacketField]
        public ushort Interval;

        [PacketField]
        public ushort Power;

        public MoveOnAngleModeRequest() : base(PacketConst.MoveOnAngleMode)
        {
        }
    }
}
