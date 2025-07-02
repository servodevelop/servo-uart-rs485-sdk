using System;
using System.Text;

namespace BrightJade
{
    public sealed class ByteBuffer
    {
        public const int DefaultBufferSize = 1024;

        public byte[] RawBuffer { get; private set; }

        private int _startIndex = 0;

        public int Position
        {
            get { return _startIndex; }
            set
            {
                if (value < 0)
                {
                    _startIndex = 0;
                }
                else if (value > _endIndex)
                {
                    _startIndex = _endIndex;
                }
                else
                {
                    _startIndex = value;
                }
            }
        }

        private int _endIndex = 0;

        public int BytesAvailable
        {
            get { return _endIndex - _startIndex; }
        }

        public int Length
        {
            get { return _endIndex; }
        }

        public int Capacity { get; private set; }

        public byte this[int index]
        {
            get
            {
                return (index < RawBuffer.Length) ? RawBuffer[index] : (byte)0;
            }

            set
            {
                if (index < RawBuffer.Length)
                {
                    RawBuffer[index] = value;
                }
            }
        }

        public ByteBuffer(int capacity = DefaultBufferSize)
        {
            Reset(capacity);
        }

        public ByteBuffer(byte[] data, bool isReference = false)
        {
            if (isReference)
            {
                _startIndex = 0;
                _endIndex = data.Length;
                Capacity = _endIndex;
                RawBuffer = data;
            }
            else
            {
                if (data.Length > DefaultBufferSize)
                {
                    Reset(data.Length);
                }
                else
                {
                    Reset();
                }

                Copy(data, 0, data.Length);
            }
        }

        public void Next(int offset = 1)
        {
            if (offset > 0)
            {
                Position += offset;
            }
        }

        public void Previous(int offset = 1)
        {
            if (offset > 0)
            {
                Position -= offset;
            }
        }

        public void Reset()
        {
            Reset(Capacity > 0 ? Capacity : DefaultBufferSize);
        }

        public void Reset(int capacity)
        {
            if (capacity > 0)
            {
                if (RawBuffer == null || RawBuffer.Length != capacity)
                {
                    Capacity = capacity;
                    RawBuffer = new byte[Capacity];
                }
                _startIndex = 0;
                _endIndex = 0;
            }
        }

        public void Extend(int extendCapacity)
        {
            if (extendCapacity > 0)
            {
                Capacity += extendCapacity;
                byte[] newBuffer = new byte[Capacity];
                Buffer.BlockCopy(RawBuffer, 0, newBuffer, 0, RawBuffer.Length);
                RawBuffer = newBuffer;
            }
        }

        public void Copy(byte[] buffer, int startIndex, int length)
        {
            if (length > Capacity)
            {
                Reset(length);
            }

            Buffer.BlockCopy(buffer, startIndex, RawBuffer, 0, length);

            _startIndex = 0;
            _endIndex = length;
        }

        public bool ReadBool()
        {
            if (RawBuffer != null && (_startIndex + 1) <= _endIndex)
            {
                bool returnValue = BitConverter.ToBoolean(RawBuffer, _startIndex);
                ++_startIndex;
                return returnValue;
            }
            else
            {
                return false;
            }
        }

        public void WriteBool(bool val)
        {
            if (RawBuffer != null && (_endIndex + 1) <= Capacity)
            {
                byte[] bytes = BitConverter.GetBytes(val);
                RawBuffer[_endIndex++] = bytes[0];
            }
        }

        public short ReadShort()
        {
            if (RawBuffer != null && (_startIndex + 2) <= _endIndex)
            {
                short returnValue = BitConverter.ToInt16(RawBuffer, _startIndex);
                _startIndex += 2;
                return returnValue;
            }
            else
            {
                return 0;
            }
        }

        public void WriteShort(short val)
        {
            if (RawBuffer != null && (_endIndex + 2) <= Capacity)
            {
                byte[] bytes = BitConverter.GetBytes(val);
                for (int i = 0; i < 2; ++i)
                {
                    RawBuffer[_endIndex++] = bytes[i];
                }
            }
        }

        public ushort ReadUShort()
        {
            if (RawBuffer != null && (_startIndex + 2) <= _endIndex)
            {
                ushort returnValue = BitConverter.ToUInt16(RawBuffer, _startIndex);
                _startIndex += 2;
                return returnValue;
            }
            else
            {
                return 0;
            }
        }

        public void WriteUShort(ushort val)
        {
            if (RawBuffer != null && (_endIndex + 2) <= Capacity)
            {
                byte[] bytes = BitConverter.GetBytes(val);
                for (int i = 0; i < 2; ++i)
                {
                    RawBuffer[_endIndex++] = bytes[i];
                }
            }
        }

        public int ReadInt()
        {
            if (RawBuffer != null && (_startIndex + 4) <= _endIndex)
            {
                int returnValue = BitConverter.ToInt32(RawBuffer, _startIndex);
                _startIndex += 4;
                return returnValue;
            }
            else
            {
                return 0;
            }
        }

        public void WriteInt(int val)
        {
            if (RawBuffer != null && (_endIndex + 4) <= Capacity)
            {
                byte[] bytes = BitConverter.GetBytes(val);
                for (int i = 0; i < 4; ++i)
                {
                    RawBuffer[_endIndex++] = bytes[i];
                }
            }
        }

        public uint ReadUInt()
        {
            if (RawBuffer != null && (_startIndex + 4) <= _endIndex)
            {
                uint returnValue = BitConverter.ToUInt32(RawBuffer, _startIndex);
                _startIndex += 4;
                return returnValue;
            }
            else
            {
                return 0;
            }
        }

        public void WriteUInt(uint val)
        {
            if (RawBuffer != null && (_endIndex + 4) <= Capacity)
            {
                byte[] bytes = BitConverter.GetBytes(val);
                for (int i = 0; i < 4; ++i)
                {
                    RawBuffer[_endIndex++] = bytes[i];
                }
            }
        }

        public long ReadLong()
        {
            if (RawBuffer != null && (_startIndex + 8) <= _endIndex)
            {
                long returnValue = BitConverter.ToInt64(RawBuffer, _startIndex);
                _startIndex += 8;
                return returnValue;
            }
            else
            {
                return 0;
            }
        }

        public void WriteLong(long val)
        {
            if (RawBuffer != null && (_endIndex + 8) <= Capacity)
            {
                byte[] bytes = BitConverter.GetBytes(val);
                for (int i = 0; i < 8; ++i)
                {
                    RawBuffer[_endIndex++] = bytes[i];
                }
            }
        }

        public ulong ReadULong()
        {
            if (RawBuffer != null && (_startIndex + 8) <= _endIndex)
            {
                ulong returnValue = BitConverter.ToUInt64(RawBuffer, _startIndex);
                _startIndex += 8;
                return returnValue;
            }
            else
            {
                return 0;
            }
        }

        public void WriteULong(ulong val)
        {
            if (RawBuffer != null && (_endIndex + 8) <= Capacity)
            {
                byte[] bytes = BitConverter.GetBytes(val);
                for (int i = 0; i < 8; ++i)
                {
                    RawBuffer[_endIndex++] = bytes[i];
                }
            }
        }

        public float ReadFloat()
        {
            if (RawBuffer != null && (_startIndex + 4) <= _endIndex)
            {
                float returnValue = BitConverter.ToSingle(RawBuffer, _startIndex);
                _startIndex += 4;
                return returnValue;
            }
            else
            {
                return 0;
            }
        }

        public void WriteFloat(float val)
        {
            if (RawBuffer != null && (_endIndex + 4) <= Capacity)
            {
                byte[] bytes = BitConverter.GetBytes(val);
                for (int i = 0; i < 4; ++i)
                {
                    RawBuffer[_endIndex++] = bytes[i];
                }
            }
        }

        public double ReadDouble()
        {
            if (RawBuffer != null && (_startIndex + 8) <= _endIndex)
            {
                double returnValue = BitConverter.ToDouble(RawBuffer, _startIndex);
                _startIndex += 8;
                return returnValue;
            }
            else
            {
                return 0;
            }
        }

        public void WriteDouble(double val)
        {
            if (RawBuffer != null && (_endIndex + 8) <= Capacity)
            {
                byte[] bytes = BitConverter.GetBytes(val);
                for (int i = 0; i < 8; ++i)
                {
                    RawBuffer[_endIndex++] = bytes[i];
                }
            }
        }

        public string ReadString(int length)
        {
            if (length > 0 && RawBuffer != null && (_startIndex + length * 2) <= _endIndex)
            {
                string returnValue = Encoding.Unicode.GetString(RawBuffer, _startIndex, length * 2).Split(new char[] { '\0' })[0];
                _startIndex += length * 2;
                return returnValue;
            }
            else
            {
                return string.Empty;
            }
        }

        public void WriteString(string val)
        {
            if (val != null && val != string.Empty &&
                RawBuffer != null && (_endIndex + val.Length * 2) <= Capacity)
            {

                byte[] bytes = Encoding.Unicode.GetBytes(val);
                for (int i = 0; i < bytes.Length; ++i)
                {
                    RawBuffer[_endIndex++] = bytes[i];
                }
            }
        }

        public byte ReadByte()
        {
            if (RawBuffer != null && _startIndex < _endIndex)
            {
                return RawBuffer[_startIndex++];
            }
            else
            {
                return 0;
            }
        }

        public void WriteByte(byte b)
        {
            if (RawBuffer != null && _endIndex < Capacity)
            {
                RawBuffer[_endIndex++] = b;
            }
        }

        public void WriteBytes(byte[] bytes)
        {
            WriteBytes(bytes, 0, bytes.Length);
        }

        public void WriteBytes(byte[] bytes, int offset, int length)
        {
            if (RawBuffer != null && (_endIndex + length) <= Capacity)
            {
                Buffer.BlockCopy(bytes, offset, RawBuffer, _endIndex, length);
                _endIndex += length;
            }
        }

        public byte[] ReadBytes(int length)
        {
            byte[] returnBuffer = null;

            if (RawBuffer != null && (_startIndex + length) <= _endIndex)
            {
                Buffer.BlockCopy(RawBuffer, _startIndex + 1, returnBuffer, 0, length);
                _startIndex += length;
            }

            return returnBuffer;
        }

        private byte[] InternalGetBuffer(int startIndex, int endIndex)
        {
            byte[] returnBuffer = null;

            if (endIndex > startIndex)
            {
                returnBuffer = new byte[endIndex - startIndex];
                Buffer.BlockCopy(RawBuffer, startIndex, returnBuffer, 0, returnBuffer.Length);
            }

            return returnBuffer;
        }

        public byte[] GetBuffer(bool isFromScratch = true)
        {
            if (isFromScratch)
            {
                return InternalGetBuffer(0, _endIndex);
            }
            else
            {
                return InternalGetBuffer(_startIndex, _endIndex);
            }
        }

        public ByteBuffer Clone()
        {
            return new ByteBuffer(GetBuffer(), true);
        }
    }
}
