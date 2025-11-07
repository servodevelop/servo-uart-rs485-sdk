using BrightJade;
using FashionStar.Servo.Uart.Protocol;
using System;
using System.Collections.Generic;

namespace FashionStar.Servo.Uart
{
    public class PacketWrapper<T> : ISyncCommandInfo where T : RequestHeader
    {
        private List<T> _container = new List<T>();

        public byte PacketID
        {
            get
            {
                if (_container.Count > 0)
                    return _container[0].PacketNumber;
                else
                    return 0;
            }
        }

        public int PacketContentLength
        {
            get
            {
                if (_container.Count > 0)
                    return PacketConverterEx.GetSize(_container[0]) - 5;
                else
                    return 0;
            }
        }

        public int Count { get => _container.Count; }

        public void Add(T packet)
        {
            _container.Add(packet);
        }

        public void Remove(int index)
        {
            _container.RemoveAt(index);
        }

        public void Clear()
        {
            _container.Clear();
        }

        public byte[] GetBytes()
        {
            if (_container.Count > 0)
            {
                int size = PacketContentLength * Count;
                byte[] bytes = new byte[size];

                int pos = 0;
                foreach (T packet in _container)
                {
                    byte[] bs = PacketConverterEx.GetBytes(packet);
                    int length = bs.Length - 5;

                    Buffer.BlockCopy(bs, 4, bytes, pos, length);
                    pos += length;
                }
                return bytes;
            }
            else
                return new byte[0];
        }
    }
}
