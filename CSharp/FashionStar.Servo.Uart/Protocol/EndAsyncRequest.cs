using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class EndAsyncRequest : RequestHeader
    {
        [PacketField]
        public byte Cancel;

        public EndAsyncRequest() : base(PacketConst.EndAsync)
        {

        }
    }
}
