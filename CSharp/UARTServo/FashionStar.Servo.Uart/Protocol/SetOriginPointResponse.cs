using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class SetOriginPointResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte result;

        public SetOriginPointResponse() : base(PacketConst.SetOriginPoint)
        {
        }
    }
}
