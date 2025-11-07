using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnAngleModeExByVelocityRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public short Angle;

        [PacketField]
        public ushort TargetVelocity;

        [PacketField]
        public ushort AccInterval;

        [PacketField]
        public ushort DecInterval;

        [PacketField]
        public ushort Power;

        public MoveOnAngleModeExByVelocityRequest() : base(PacketConst.MoveOnAngleModeExByVelocity)
        {
        }
    }
}