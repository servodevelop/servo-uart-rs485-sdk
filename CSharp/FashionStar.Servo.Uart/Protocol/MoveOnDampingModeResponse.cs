using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnDampingModeResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public MoveOnDampingModeResponse() : base(PacketConst.MoveOnDampingMode)
        {
        }
    }
}
