using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnAngleModeExByVelocityResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public MoveOnAngleModeExByVelocityResponse() : base(PacketConst.MoveOnAngleModeExByVelocity)
        {
        }
    }
}