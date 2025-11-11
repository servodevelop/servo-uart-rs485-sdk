using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnWheelModeRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Method;

        [PacketField]
        public ushort Speed;

        [PacketField]
        public ushort Value;

        public MoveOnWheelModeRequest() : base(PacketConst.MoveOnWheelMode)
        {
        }
    }
}
