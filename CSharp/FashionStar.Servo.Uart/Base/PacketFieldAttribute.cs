using System;

namespace BrightJade
{
    [AttributeUsage(AttributeTargets.Property | AttributeTargets.Field, Inherited = true, AllowMultiple = false)]
    public sealed class PacketFieldAttribute : Attribute
    {
        public bool IsCustomType { get; set; }

        public bool IsDynamicSize { get; set; }

        public int Size { get; set; }

        public int Count { get; set; }

        public string SizeBy { get; set; }

        public string CountBy { get; set; }

        public int Order { get; set; }

        public int StartIndex { get; set; }

        public PacketFieldAttribute(int order = 0)
        {
            Size = 0;
            Count = 1;
            SizeBy = string.Empty;
            CountBy = string.Empty;
            Order = order;
            StartIndex = 0;
            IsCustomType = false;
            IsDynamicSize = false;
        }
    }
}
