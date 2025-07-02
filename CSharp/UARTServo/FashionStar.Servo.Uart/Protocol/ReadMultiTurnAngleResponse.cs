using BrightJade;
namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ReadMultiTurnAngleResponse : ResponseHeader
    {
        [PacketField]
        public byte ID;

        [PacketField]
        public int Angle;

        [PacketField]
        public short Turns;

        public ReadMultiTurnAngleResponse() : base(PacketConst.ReadMultiTurnAngle)
        {
        }
    }
}
