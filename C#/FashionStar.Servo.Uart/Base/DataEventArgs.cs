using System;

namespace BrightJade
{
    public class DataEventArgs<TDataType> : EventArgs
    {
        public TDataType Data { get; protected set; }

        public DataEventArgs(TDataType data)
        {
            Data = data;
        }
    }
}
