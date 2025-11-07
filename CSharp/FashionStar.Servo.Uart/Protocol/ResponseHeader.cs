using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    [PacketSerializable]
    public class ResponseHeader
    {
        [PacketField(int.MinValue)]
        public ushort CheckCode = 0x1c05;

        [PacketField(int.MinValue + 1)]
        public byte PacketNumber = 0;

        [PacketField(int.MinValue + 2)]
        public byte PacketLength = 0;

        [PacketField(int.MaxValue)]
        public byte Checksum = 0;

        public ResponseHeader(byte packetNumber = 0)
        {
            PacketNumber = packetNumber;
        }
    }
}
