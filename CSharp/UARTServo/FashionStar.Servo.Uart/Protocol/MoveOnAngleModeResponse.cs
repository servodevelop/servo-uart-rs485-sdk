using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnAngleModeResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public MoveOnAngleModeResponse() : base(PacketConst.MoveOnAngleMode)
        {
        }
    }
}
