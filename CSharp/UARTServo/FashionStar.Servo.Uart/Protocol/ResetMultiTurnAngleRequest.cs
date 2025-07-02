using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ResetMultiTurnAngleRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        public ResetMultiTurnAngleRequest() : base(PacketConst.ResetMultiTurnAngle)
        {
        }
    }
}
