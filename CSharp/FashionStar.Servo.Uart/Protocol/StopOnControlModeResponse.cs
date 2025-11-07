using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class StopOnControlModeResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Result;

        public StopOnControlModeResponse() : base(PacketConst.StopOnControlMode)
        {

        }
    }
}
