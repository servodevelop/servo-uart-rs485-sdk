using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnMultiTurnAngleModeRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public int Angle;

        [PacketField]
        public uint Interval;

        [PacketField]
        public ushort Power;

        public MoveOnMultiTurnAngleModeRequest() : base(PacketConst.MoveOnMultiTurnAngleMode)
        {

        }
    }
}
