using BrightJade;
using FashionStar.Servo.Uart.Protocol;
using System;

namespace FashionStar.Servo.Uart
{
    partial class ServoController
    {
        private ushort _headerCode = 0x1c05;
        private int _minPacketLength = 5;
        private ByteBuffer _dataBuffer = null;

        public event EventHandler<DataEventArgs<PingResponse>> PingResponsed;
        public event EventHandler<DataEventArgs<ResetUserDataResponse>> ResetUserDataResponsed;
        public event EventHandler<DataEventArgs<ReadDataResponse>> ReadDataResponsed;
        public event EventHandler<DataEventArgs<WriteDataResponse>> WriteDataResponsed;
        public event EventHandler<DataEventArgs<ReadBatchDataResponse>> ReadBatchDataResponsed;
        public event EventHandler<DataEventArgs<WriteBatchDataResponse>> WriteBatchDataResponsed;
        public event EventHandler<DataEventArgs<MoveOnWheelModeResponse>> MoveOnWheelModeResponsed;
        public event EventHandler<DataEventArgs<MoveOnAngleModeResponse>> MoveOnAngleModeResponsed;
        public event EventHandler<DataEventArgs<MoveOnAngleModeExByIntervalResponse>> MoveOnAngleModeExByTimeResponsed;
        public event EventHandler<DataEventArgs<MoveOnAngleModeExByVelocityResponse>> MoveOnAngleModeExByVelocityResponsed;
        public event EventHandler<DataEventArgs<MoveOnDampingModeResponse>> MoveOnDampingModeResponsed;
        public event EventHandler<DataEventArgs<ReadAngleResponse>> ReadAngleResponsed;
        public event EventHandler<DataEventArgs<MoveOnMultiTurnAngleModeResponse>> MoveOnMultiTurnAngleModeResponsed;
        public event EventHandler<DataEventArgs<MoveOnMultiTurnAngleModeExByIntervalResponse>> MoveOnMultiTurnAngleModeExByTimeResponsed;
        public event EventHandler<DataEventArgs<MoveOnMultiTurnAngleModeExByVelocityResponse>> MoveOnMultiTurnAngleModeExByVelocityResponsed;
        public event EventHandler<DataEventArgs<ReadMultiTurnAngleResponse>> ReadMultiTurnAngleResponsed;
        public event EventHandler<DataEventArgs<ResetMultiTurnAngleResponse>> ResetMultiTurnAngleResponsed;

        protected virtual void OnPingResponsed(DataEventArgs<PingResponse> e)
        {
            PingResponsed?.Invoke(this, e);
        }

        protected virtual void OnResetUserDataResponsed(DataEventArgs<ResetUserDataResponse> e)
        {
            ResetUserDataResponsed?.Invoke(this, e);
        }

        protected virtual void OnReadDataResponsed(DataEventArgs<ReadDataResponse> e)
        {
            ReadDataResponsed?.Invoke(this, e);
        }

        protected virtual void OnWriteDataResponsed(DataEventArgs<WriteDataResponse> e)
        {
            WriteDataResponsed?.Invoke(this, e);
        }

        protected virtual void OnReadBatchDataResponsed(DataEventArgs<ReadBatchDataResponse> e)
        {
            ReadBatchDataResponsed?.Invoke(this, e);
        }

        protected virtual void OnWriteBatchDataResponsed(DataEventArgs<WriteBatchDataResponse> e)
        {
            WriteBatchDataResponsed?.Invoke(this, e);
        }

        protected virtual void OnMoveOnWheelModeResponsed(DataEventArgs<MoveOnWheelModeResponse> e)
        {
            MoveOnWheelModeResponsed?.Invoke(this, e);
        }

        protected virtual void OnMoveOnAngleModeResponsed(DataEventArgs<MoveOnAngleModeResponse> e)
        {
            MoveOnAngleModeResponsed?.Invoke(this, e);
        }

        protected virtual void OnMoveOnAngleModeExByTimeResponsed(DataEventArgs<MoveOnAngleModeExByIntervalResponse> e)
        {
            MoveOnAngleModeExByTimeResponsed?.Invoke(this, e);
        }

        protected virtual void OnMoveOnAngleModeExByVelocityResponsed(DataEventArgs<MoveOnAngleModeExByVelocityResponse> e)
        {
            MoveOnAngleModeExByVelocityResponsed?.Invoke(this, e);
        }

        protected virtual void OnMoveOnDampingModeResponsed(DataEventArgs<MoveOnDampingModeResponse> e)
        {
            MoveOnDampingModeResponsed?.Invoke(this, e);
        }

        protected virtual void OnReadAngleResponsed(DataEventArgs<ReadAngleResponse> e)
        {
            ReadAngleResponsed?.Invoke(this, e);
        }

        protected virtual void OnMoveOnMultiTurnAngleModeResponsed(DataEventArgs<MoveOnMultiTurnAngleModeResponse> e)
        {
            MoveOnMultiTurnAngleModeResponsed?.Invoke(this, e);
        }

        protected virtual void OnMoveOnMultiTurnAngleModeExByTimeResponsed(DataEventArgs<MoveOnMultiTurnAngleModeExByIntervalResponse> e)
        {
            MoveOnMultiTurnAngleModeExByTimeResponsed?.Invoke(this, e);
        }

        protected virtual void OnMoveOnMultiTurnAngleModeExByVelocityResponsed(DataEventArgs<MoveOnMultiTurnAngleModeExByVelocityResponse> e)
        {
            MoveOnMultiTurnAngleModeExByVelocityResponsed?.Invoke(this, e);
        }

        protected virtual void OnReadMultiTurnAngleResponsed(DataEventArgs<ReadMultiTurnAngleResponse> e)
        {
            ReadMultiTurnAngleResponsed?.Invoke(this, e);
        }

        protected virtual void OnResetMultiTurnAngleResponsed(DataEventArgs<ResetMultiTurnAngleResponse> e)
        {
            ResetMultiTurnAngleResponsed?.Invoke(this, e);
        }

        public void AppendData(byte[] data)
        {
            // First time, or all data has been processed.
            if (_dataBuffer == null || _dataBuffer.BytesAvailable == 0)
            {
                // For efficiency reasons, refer to the buffer without copying the data.
                _dataBuffer = new ByteBuffer(data, true);
            }
            else
            {
                _dataBuffer.Extend(data.Length);
                _dataBuffer.WriteBytes(data);
            }

            while (_dataBuffer.BytesAvailable >= _minPacketLength)
            {
                ushort headerCode = _dataBuffer.ReadUShort();
                if (headerCode == _headerCode)
                {
                    byte packetNumber = _dataBuffer.ReadByte();
                    byte packetLength = _dataBuffer.ReadByte();

                    _dataBuffer.Previous(4);

                    if (_dataBuffer.BytesAvailable >= packetLength + 5) // + 5 for header & checksum length.
                    {
                        switch (packetNumber)
                        {
                            default:
                                {
                                    _dataBuffer.Next(packetLength + 1);
                                    break;
                                }
                            case PacketConst.Ping:
                                {
                                    PingResponse packet = PacketConverterEx.GetObject<PingResponse>(_dataBuffer);
                                    OnPingResponsed(new DataEventArgs<PingResponse>(packet));
                                }
                                break;
                            case PacketConst.ResetUserData:
                                {
                                    ResetUserDataResponse packet = PacketConverterEx.GetObject<ResetUserDataResponse>(_dataBuffer);
                                    OnResetUserDataResponsed(new DataEventArgs<ResetUserDataResponse>(packet));
                                }
                                break;
                            case PacketConst.ReadData:
                                {
                                    ReadDataResponse packet = PacketConverterEx.GetObject<ReadDataResponse>(_dataBuffer);

                                    switch (packet.DataID)
                                    {
                                        case DataConst.Status:
                                            {
                                                packet.Value = packet.Data[0];
                                            }
                                            break;
                                        case DataConst.Voltage:
                                        case DataConst.Current:
                                        case DataConst.Power:
                                        case DataConst.Temperature:
                                        case DataConst.Model:
                                        case DataConst.Version:
                                            {
                                                packet.Value = BitConverter.ToUInt16(packet.Data, 0);
                                            }
                                            break;
                                        case DataConst.SerialNumber:
                                            {
                                                packet.Value = BitConverter.ToUInt32(packet.Data, 0);
                                            }
                                            break;
                                    }

                                    OnReadDataResponsed(new DataEventArgs<ReadDataResponse>(packet));
                                }
                                break;
                            case PacketConst.WriteData:
                                {
                                    WriteDataResponse packet = PacketConverterEx.GetObject<WriteDataResponse>(_dataBuffer);
                                    OnWriteDataResponsed(new DataEventArgs<WriteDataResponse>(packet));
                                }
                                break;
                            case PacketConst.ReadBatchData:
                                {
                                    ReadBatchDataResponse packet = PacketConverterEx.GetObject<ReadBatchDataResponse>(_dataBuffer);
                                    OnReadBatchDataResponsed(new DataEventArgs<ReadBatchDataResponse>(packet));
                                }
                                break;
                            case PacketConst.WriteBatchData:
                                {
                                    WriteBatchDataResponse packet = PacketConverterEx.GetObject<WriteBatchDataResponse>(_dataBuffer);
                                    OnWriteBatchDataResponsed(new DataEventArgs<WriteBatchDataResponse>(packet));
                                }
                                break;
                            case PacketConst.MoveOnWheelMode:
                                {
                                    MoveOnWheelModeResponse packet = PacketConverterEx.GetObject<MoveOnWheelModeResponse>(_dataBuffer);
                                    OnMoveOnWheelModeResponsed(new DataEventArgs<MoveOnWheelModeResponse>(packet));
                                }
                                break;
                            case PacketConst.MoveOnAngleMode:
                                {
                                    MoveOnAngleModeResponse packet = PacketConverterEx.GetObject<MoveOnAngleModeResponse>(_dataBuffer);
                                    OnMoveOnAngleModeResponsed(new DataEventArgs<MoveOnAngleModeResponse>(packet));
                                }
                                break;
                            case PacketConst.MoveOnAngleModeExByInterval:
                                {
                                    MoveOnAngleModeExByIntervalResponse packet = PacketConverterEx.GetObject<MoveOnAngleModeExByIntervalResponse>(_dataBuffer);
                                    OnMoveOnAngleModeExByTimeResponsed(new DataEventArgs<MoveOnAngleModeExByIntervalResponse>(packet));
                                }
                                break;
                            case PacketConst.MoveOnAngleModeExByVelocity:
                                {
                                    MoveOnAngleModeExByVelocityResponse packet = PacketConverterEx.GetObject<MoveOnAngleModeExByVelocityResponse>(_dataBuffer);
                                    OnMoveOnAngleModeExByVelocityResponsed(new DataEventArgs<MoveOnAngleModeExByVelocityResponse>(packet));
                                }
                                break;
                            case PacketConst.MoveOnDampingMode:
                                {
                                    MoveOnDampingModeResponse packet = PacketConverterEx.GetObject<MoveOnDampingModeResponse>(_dataBuffer);
                                    OnMoveOnDampingModeResponsed(new DataEventArgs<MoveOnDampingModeResponse>(packet));
                                }
                                break;
                            case PacketConst.ReadAngle:
                                {
                                    ReadAngleResponse packet = PacketConverterEx.GetObject<ReadAngleResponse>(_dataBuffer);
                                    OnReadAngleResponsed(new DataEventArgs<ReadAngleResponse>(packet));
                                }
                                break;
                            case PacketConst.MoveOnMultiTurnAngleMode:
                                {
                                    MoveOnMultiTurnAngleModeResponse packet = PacketConverterEx.GetObject<MoveOnMultiTurnAngleModeResponse>(_dataBuffer);
                                    OnMoveOnMultiTurnAngleModeResponsed(new DataEventArgs<MoveOnMultiTurnAngleModeResponse>(packet));
                                }
                                break;
                            case PacketConst.MoveOnMultiTurnAngleModeExByInterval:
                                {
                                    MoveOnMultiTurnAngleModeExByIntervalResponse packet = PacketConverterEx.GetObject<MoveOnMultiTurnAngleModeExByIntervalResponse>(_dataBuffer);
                                    OnMoveOnMultiTurnAngleModeExByTimeResponsed(new DataEventArgs<MoveOnMultiTurnAngleModeExByIntervalResponse>(packet));
                                }
                                break;
                            case PacketConst.MoveOnMultiTurnAngleModeExByVelocity:
                                {
                                    MoveOnMultiTurnAngleModeExByVelocityResponse packet = PacketConverterEx.GetObject<MoveOnMultiTurnAngleModeExByVelocityResponse>(_dataBuffer);
                                    OnMoveOnMultiTurnAngleModeExByVelocityResponsed(new DataEventArgs<MoveOnMultiTurnAngleModeExByVelocityResponse>(packet));
                                }
                                break;
                            case PacketConst.ReadMultiTurnAngle:
                                {
                                    ReadMultiTurnAngleResponse packet = PacketConverterEx.GetObject<ReadMultiTurnAngleResponse>(_dataBuffer);
                                    OnReadMultiTurnAngleResponsed(new DataEventArgs<ReadMultiTurnAngleResponse>(packet));
                                }
                                break;
                            case PacketConst.ResetMultiTurnAngle:
                                {
                                    ResetMultiTurnAngleResponse packet = PacketConverterEx.GetObject<ResetMultiTurnAngleResponse>(_dataBuffer);
                                    OnResetMultiTurnAngleResponsed(new DataEventArgs<ResetMultiTurnAngleResponse>(packet));
                                }
                                break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    // Discard 1 byte.
                    _dataBuffer.Previous(1);
                }
            }
        }

    }
}
