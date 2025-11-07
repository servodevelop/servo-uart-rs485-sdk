using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnMultiTurnAngleModeResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public MoveOnMultiTurnAngleModeResponse() : base(PacketConst.MoveOnMultiTurnAngleMode)
        {
        }
    }
}
