using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnAngleModeExByIntervalRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public short Angle;

        [PacketField]
        public ushort Interval;

        [PacketField]
        public ushort AccInterval;

        [PacketField]
        public ushort DecInterval;

        [PacketField]
        public ushort Power;

        public MoveOnAngleModeExByIntervalRequest() : base(PacketConst.MoveOnAngleModeExByInterval)
        {
        }
    }
}
