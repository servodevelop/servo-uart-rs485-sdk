using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class StopOnControlModeRequest : RequestHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public byte Method = 0x10;

        [PacketField]
        public ushort Power;

        public StopOnControlModeRequest() : base(PacketConst.StopOnControlMode)
        {

        }
    }
}
