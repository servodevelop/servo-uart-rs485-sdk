using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ReadAngleRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        public ReadAngleRequest() : base(PacketConst.ReadAngle)
        {
        }
    }
}
