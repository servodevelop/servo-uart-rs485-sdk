using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class BeginAsyncRequest : RequestHeader
    {
        public BeginAsyncRequest() : base(PacketConst.BeginAsync)
        {

        }
    }
}
