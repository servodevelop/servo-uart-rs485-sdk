using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class BoardPowerControlRequest : RequestHeader
    {
        [PacketField]
        public byte Power = 1;

        public BoardPowerControlRequest() : base(PacketConst.BoardPowerControl)
        {
        }
    }
}
