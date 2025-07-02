using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnMultiTurnAngleModeExByIntervalResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public MoveOnMultiTurnAngleModeExByIntervalResponse() : base(PacketConst.MoveOnMultiTurnAngleModeExByInterval)
        {
        }
    }
}
