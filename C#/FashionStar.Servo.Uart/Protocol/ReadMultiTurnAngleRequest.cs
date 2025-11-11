using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ReadMultiTurnAngleRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        public ReadMultiTurnAngleRequest() : base(PacketConst.ReadMultiTurnAngle)
        {
        }
    }
}
