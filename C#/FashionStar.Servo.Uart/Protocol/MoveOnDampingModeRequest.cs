using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class MoveOnDampingModeRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public ushort Power;

        public MoveOnDampingModeRequest() : base(PacketConst.MoveOnDampingMode)
        {
        }
    }
}
