using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{ 
    [PacketSerializable]
    public class SetOriginPointRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Reset;

        public SetOriginPointRequest() : base(PacketConst.SetOriginPoint)
        {
        }
    }
}
