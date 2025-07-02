using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ReadAngleResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public short Angle;

        public ReadAngleResponse() : base(PacketConst.ReadAngle)
        {
        }
    }
}
