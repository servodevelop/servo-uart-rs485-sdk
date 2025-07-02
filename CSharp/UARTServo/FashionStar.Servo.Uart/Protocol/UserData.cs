using BrightJade;
using System.Xml.Serialization;

namespace FashionStar.Servo.Uart.Protocol
{
    [XmlRootAttribute("Parameters")]
    [PacketSerializable]
    public class UserData
    {
        /// <summary>
        /// 保留。
        /// </summary>
        [XmlIgnore]
        [PacketField]
        public byte Reserved = 1;

        /// <summary>
        /// 控制回應。
        /// </summary>
        [PacketField]
        public byte IsResponse = 0;

        /// <summary>
        /// 舵機編號。
        /// </summary>
        [XmlIgnore]
        [PacketField]
        public byte ID = 0;

        /// <summary>
        /// 控制模式。
        /// </summary>
        [PacketField]
        public byte ControlMode = 0;

        /// <summary>
        /// 串列傳輸速率。
        /// </summary>
        [PacketField]
        [XmlIgnore]
        public byte BaudIndex = 5;

        /// <summary>
        /// 堵轉保護功能。
        /// </summary>
        [PacketField]
        public byte StallProtect = 0;

        /// <summary>
        /// 堵轉功率上限。
        /// </summary>
        [PacketField]
        public ushort StallPowerLimit = 10000;

        /// <summary>
        /// 低壓保護電壓。
        /// </summary>
        [PacketField]
        public ushort OverVoltageLowLevel = 4000;

        /// <summary>
        /// 高壓保護電壓。
        /// </summary>
        [PacketField]
        public ushort OverVoltageHighLevel = 8400;

        /// <summary>
        /// 溫度保護值。
        /// </summary>
        [PacketField]
        public ushort OverTemperatureTriggerLevel = 651;

        /// <summary>
        /// 功率保護值。
        /// </summary>
        [PacketField]
        public ushort OverPowerTriggerLevel = 10000;

        /// <summary>
        /// 電流保護值。
        /// </summary>
        [PacketField]
        public ushort OverCurrentTriggerLevel = 4000;

        /// <summary>
        /// 加速度。
        /// </summary>
        [PacketField]
        public byte StartupSpeed = 0;

        /// <summary>
        /// 減速度。
        /// </summary>
        [PacketField]
        public byte BrakeSpeed = 0;

        /// <summary>
        /// 上電鎖力開關。
        /// </summary>
        [PacketField]
        public byte PowerLockSwitch = 0;

        /// <summary>
        /// 輪式模式刹車開關。
        /// </summary>
        [PacketField]
        public byte WheelModeBrakeSwitch = 0;

        /// <summary>
        /// 角度限制開關。
        /// </summary>
        [PacketField]
        public byte AngleLimitSwitch = 1;

        /// <summary>
        /// 上電緩啟動開關。
        /// </summary>
        [PacketField]
        public byte SoftStartSwitch = 1;

        /// <summary>
        /// 上電緩啟動時間。
        /// </summary>
        [PacketField]
        public ushort SoftStartTime = 3000;

        /// <summary>
        /// 角度上限。
        /// </summary>
        [PacketField]
        public short AngleUpperLimit = 1350;

        /// <summary>
        /// 角度下限。
        /// </summary>
        [PacketField]
        public short AngleLowerLimit = -1350;

        //
        /// <summary>
        /// 中點偏移。
        /// </summary>
        [PacketField]
        public short CenterPointOffset = 0;
    }
}
