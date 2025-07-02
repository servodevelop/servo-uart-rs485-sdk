using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnMultiTurnAngleModeExByVelocityRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public int Angle;

        [PacketField]
        public ushort TargetVelocity;

        [PacketField]
        public ushort AccInterval;

        [PacketField]
        public ushort DecInterval;

        [PacketField]
        public ushort Power;

        public MoveOnMultiTurnAngleModeExByVelocityRequest() : base(PacketConst.MoveOnMultiTurnAngleModeExByVelocity)
        {
        }
    }
}
