using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnMultiTurnAngleModeExByVelocityResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public MoveOnMultiTurnAngleModeExByVelocityResponse() : base(PacketConst.MoveOnMultiTurnAngleModeExByVelocity)
        {
        }
    }
}
