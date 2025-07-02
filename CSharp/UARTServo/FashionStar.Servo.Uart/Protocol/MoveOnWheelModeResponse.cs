using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnWheelModeResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public MoveOnWheelModeResponse() : base(PacketConst.MoveOnWheelMode)
        {
        }
    }
}
