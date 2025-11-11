using BrightJade;
using FashionStar.Servo.Uart.Protocol;
using System;

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
            StopOnControlMode(id);
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

        public void BeginAsync(byte id)
        {
            BeginAsyncRequest packet = new BeginAsyncRequest();
            PacketConvert(packet);
        }

        public void EndAsync(bool cancel = false)
        {
            EndAsyncRequest packet = new EndAsyncRequest();
            packet.Cancel = (byte)(cancel ? 1 : 0);
            PacketConvert(packet);
        }

        public void Monitor(byte id)
        {
            MonitorRequest packet = new MonitorRequest();
            packet.ID = id;
            PacketConvert(packet);
        }

        public void SetOriginPoint(byte id, bool isReset = false)
        {
            SetOriginPointRequest packet = new SetOriginPointRequest();
            packet.ID = id;
            packet.Reset = (byte)(isReset ? 1 : 0);
            PacketConvert(packet);
        }

        public void StopOnControlMode(byte id, byte method = 0x10, ushort power = 0)
        {
            StopOnControlModeRequest packet = new StopOnControlModeRequest();
            packet.ID = id;
            packet.Method = method;
            packet.Power = power;
            PacketConvert(packet);
        }

        public void SyncCommand(byte commandID, int length, int count, byte[] data)
        {
            if (data.Length == length * count)
            {
                if (data.Length < 255)
                {
                    SyncCommandRequest packet = new SyncCommandRequest();
                    packet.ID = commandID;
                    packet.Length = (byte)length;
                    packet.Count = (byte)count;
                    packet.Data = data;
                    PacketConvert(packet);
                }
                else
                {
                    SyncCommandRequestEx packet = new SyncCommandRequestEx();
                    packet.ID = commandID;
                    packet.Length = (byte)length;
                    packet.Count = (byte)count;
                    packet.Data = data;
                    PacketConvert(packet);
                }
            }
        }

        public void SyncCommand(ISyncCommandInfo info)
        {
            SyncCommand(info.PacketID, info.PacketContentLength, info.Count, info.GetBytes());
        }

        private void PacketConvert(object packet)
        {
            byte[] data = PacketConverterEx.GetBytes(packet);

            // Packet length.
            if (data.Length - 5 > 255 && data.Length - 7 > 255)
            {
                byte[] length = BitConverter.GetBytes(data.Length - 7);
                data[3] = 255;
                data[4] = length[0];
                data[5] = length[1];
            }
            else
            {
                data[3] = (byte)(data.Length - 5);
            }

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
    }
}
