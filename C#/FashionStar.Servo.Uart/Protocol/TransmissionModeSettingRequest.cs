using BrightJade;

namespace FashionStar.Servo.Uart.Protocol
{
    /// <summary>
    /// 客戶端請求設定傳輸模式。
    /// </summary>
    [PacketSerializable]
    public class TransmissionModeSettingRequest
    {
        [PacketField(1)]
        public int CheckCode = 0x1d054c13;

        [PacketField(2)]
        public ushort PacketNumber = 10010;

        [PacketField(3)]
        public ushort PacketLength = 1;

        /// <summary>
        /// 0:舊通訊協定；1:新通訊協定。
        /// </summary>
        [PacketField(4)]
        public byte Mode = 0xaa;

        [PacketField(5)]
        public byte Checksum = 0;
    }
}
