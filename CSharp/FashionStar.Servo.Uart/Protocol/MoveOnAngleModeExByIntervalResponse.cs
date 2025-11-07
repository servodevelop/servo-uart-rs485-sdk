using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnAngleModeExByIntervalResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public MoveOnAngleModeExByIntervalResponse() : base(PacketConst.MoveOnAngleModeExByInterval)
        {
        }
    }
}