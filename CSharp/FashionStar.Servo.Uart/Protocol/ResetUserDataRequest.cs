using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ResetUserDataRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        public ResetUserDataRequest() : base(PacketConst.ResetUserData)
        {
        }
    }
}
