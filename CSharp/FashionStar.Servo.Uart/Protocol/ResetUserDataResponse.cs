using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ResetUserDataResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public ResetUserDataResponse() : base(PacketConst.ResetUserData)
        {
        }
    }
}
