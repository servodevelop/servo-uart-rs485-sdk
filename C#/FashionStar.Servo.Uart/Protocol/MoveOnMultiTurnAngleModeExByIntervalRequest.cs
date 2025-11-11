using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnMultiTurnAngleModeExByIntervalRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public int Angle;

        [PacketField]
        public uint Interval;

        [PacketField]
        public ushort AccInterval;

        [PacketField]
        public ushort DecInterval;

        [PacketField]
        public ushort Power;

        public MoveOnMultiTurnAngleModeExByIntervalRequest() : base(PacketConst.MoveOnMultiTurnAngleModeExByInterval)
        {
        }
    }
}
