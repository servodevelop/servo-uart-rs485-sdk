using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ResetMultiTurnAngleResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public ResetMultiTurnAngleResponse() : base(PacketConst.ResetMultiTurnAngle)
        {
        }
    }
}
