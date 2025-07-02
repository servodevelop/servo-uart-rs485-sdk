using BrightJade;
using FashionStar.Servo.Uart.Protocol;

namespace FashionStar.Servo.Uart
{
    partial class ServoController
    {
        public void SetNewConnectionMode()
        {
            TransmissionModeSettingRequest packet = new TransmissionModeSettingRequest();
            byte[] data = PacketConverterEx.GetBytes(packet);

            data.AppendCheckSum();

            WriteData(data);
        }

        public void TurnOnBoardPower()
        {
            BoardPowerControlRequest packet = new BoardPowerControlRequest();
            PacketConvert(packet);
        }

        public void Stop(byte id)
        {
            MoveOnWheelMode(id, 0, 0, 0);
        }

        public void Ping(byte id)
        {
            PingRequest packet = new PingRequest();
            packet.ID = id;
            PacketConvert(packet);
        }

        public void ResetUserData(byte id)
        {
            ResetUserDataRequest packet = new ResetUserDataRequest();
            packet.ID = id;
            PacketConvert(packet);
        }

        public void ReadData(byte id, byte dataID)
        {
            ReadDataRequest packet = new ReadDataRequest();
            packet.ID = id;
            packet.DataID = dataID;
            PacketConvert(packet);
        }

        public void WriteData(byte id, byte dataID, byte[] data)
        {
            WriteDataRequest packet = new WriteDataRequest();
            packet.ID = id;
            packet.DataID = dataID;
            packet.Data = data;
            PacketConvert(packet);
        }

        public void ReadBatchData(byte id)
        {
            ReadBatchDataRequest packet = new ReadBatchDataRequest();
            packet.ID = id;
            PacketConvert(packet);
        }

        public void WriteBatchData(byte id, UserData data)
        {
            WriteBatchDataRequest packet = new WriteBatchDataRequest();
            packet.ID = id;
            packet.Data = data;
            PacketConvert(packet);
        }

        public void MoveOnWheelMode(byte id, byte method, ushort speed, ushort value)
        {
            MoveOnWheelModeRequest packet = new MoveOnWheelModeRequest();
            packet.ID = id;
            packet.Method = method;
            packet.Speed = speed;
            packet.Value = value;
            PacketConvert(packet);
        }

        public void MoveOnAngleMode(byte id, short angle, ushort interval, ushort power = 0)
        {
            MoveOnAngleModeRequest packet = new MoveOnAngleModeRequest();
            packet.ID = id;
            packet.Angle = angle;
            packet.Interval = interval;
            packet.Power = power;
            PacketConvert(packet);
        }

        public void MoveOnAngleModeExByInterval(byte id, short angle, ushort interval, ushort accInterval, ushort decInterval, ushort power = 0)
        {
            MoveOnAngleModeExByIntervalRequest packet = new MoveOnAngleModeExByIntervalRequest();
            packet.ID = id;
            packet.Angle = angle;
            packet.Interval = interval;
            packet.AccInterval = accInterval;
            packet.DecInterval = decInterval;
            packet.Power = power;
            PacketConvert(packet);
        }

        public void MoveOnAngleModeExByVelocity(byte id, short angle, ushort targetVelocity, ushort accInterval, ushort decInterval, ushort power = 0)
        {
            MoveOnAngleModeExByVelocityRequest packet = new MoveOnAngleModeExByVelocityRequest();
            packet.ID = id;
            packet.Angle = angle;
            packet.TargetVelocity = targetVelocity;
            packet.AccInterval = accInterval;
            packet.DecInterval = decInterval;
            packet.Power = power;
            PacketConvert(packet);
        }

        public void MoveOnDampingMode(byte id, ushort power = 0)
        {
            MoveOnDampingModeRequest packet = new MoveOnDampingModeRequest();
            packet.ID = id;
            packet.Power = power;
            PacketConvert(packet);
        }

        public void ReadAngle(byte id)
        {
            ReadAngleRequest packet = new ReadAngleRequest();
            packet.ID = id;
            PacketConvert(packet);
        }

        public void MoveOnMultiTurnAngleMode(byte id, int angle, uint interval, ushort power = 0)
        {
            MoveOnMultiTurnAngleModeRequest packet = new MoveOnMultiTurnAngleModeRequest();
            packet.ID = id;
            packet.Angle = angle;
            packet.Interval = interval;
            packet.Power = power;
            PacketConvert(packet);
        }

        public void MoveOnMultiTurnAngleModeExByInterval(byte id, int angle, uint interval, ushort accInterval, ushort decInterval, ushort power = 0)
        {
            MoveOnMultiTurnAngleModeExByIntervalRequest packet = new MoveOnMultiTurnAngleModeExByIntervalRequest();
            packet.ID = id;
            packet.Angle = angle;
            packet.Interval = interval;
            packet.AccInterval = accInterval;
            packet.DecInterval = decInterval;
            packet.Power = power;
            PacketConvert(packet);
        }

        public void MoveOnMultiTurnAngleModeExByVelocity(byte id, int angle, ushort targetVelocity, ushort accInterval, ushort decInterval, ushort power = 0)
        {
            MoveOnMultiTurnAngleModeExByVelocityRequest packet = new MoveOnMultiTurnAngleModeExByVelocityRequest();
            packet.ID = id;
            packet.Angle = angle;
            packet.TargetVelocity = targetVelocity;
            packet.AccInterval = accInterval;
            packet.DecInterval = decInterval;
            packet.Power = power;
            PacketConvert(packet);
        }

        public void ReadMultiTurnAngle(byte id)
        {
            ReadMultiTurnAngleRequest packet = new ReadMultiTurnAngleRequest();
            packet.ID = id;
            PacketConvert(packet);
        }

        public void ResetMultiTurnAngle(byte id)
        {
            ResetMultiTurnAngleRequest packet = new ResetMultiTurnAngleRequest();
            packet.ID = id;
            PacketConvert(packet);
        }

        private void PacketConvert(object packet)
        {
            byte[] data = PacketConverterEx.GetBytes(packet);

            // Packet length.
            data[3] = (byte)(data.Length - 5);

            data.AppendCheckSum();

            WriteData(data);
        }

        private void WriteData(byte[] data)
        {
            if (SerialPortManager != null)
            {
                SerialPortManager.Write(data, 0, data.Length);
            }
        }
        public void SetOriginPoint(byte id)
        {
            SetOriginPointRequest packet = new SetOriginPointRequest();
            packet.ID = id;
            PacketConvert(packet);
        }
    }
}
