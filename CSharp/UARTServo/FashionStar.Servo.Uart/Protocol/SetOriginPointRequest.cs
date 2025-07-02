using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class SetOriginPointRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte reset = 0;

        public SetOriginPointRequest() : base(PacketConst.SetOriginPoint)
        {
        }
    }
}
