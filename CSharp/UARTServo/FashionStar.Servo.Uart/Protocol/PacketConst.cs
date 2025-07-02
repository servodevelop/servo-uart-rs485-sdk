namespace FashionStar.Servo.Uart.Protocol
{
    public class PacketConst
    {
        public const byte Ping = 1;
        public const byte ResetUserData = 2;
        public const byte ReadData = 3;
        public const byte WriteData = 4;
        public const byte ReadBatchData = 5;
        public const byte WriteBatchData = 6;
        public const byte MoveOnWheelMode = 7;
        public const byte MoveOnAngleMode = 8;
        public const byte MoveOnDampingMode = 9;
        public const byte ReadAngle = 10;
        public const byte MoveOnAngleModeExByInterval = 11;
        public const byte MoveOnAngleModeExByVelocity = 12;
        public const byte MoveOnMultiTurnAngleMode = 13;
        public const byte MoveOnMultiTurnAngleModeExByInterval = 14;
        public const byte MoveOnMultiTurnAngleModeExByVelocity = 15;
        public const byte ReadMultiTurnAngle = 16;
        public const byte ResetMultiTurnAngle = 17;
        public const byte SetOriginPoint = 23;        
        public const byte BoardPowerControl = 0xf3;
    }
}
