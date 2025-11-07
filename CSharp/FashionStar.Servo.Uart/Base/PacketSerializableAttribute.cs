using System;

namespace BrightJade
{
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct, Inherited = true, AllowMultiple = false)]
    public sealed class PacketSerializableAttribute : Attribute
    {
        public bool FieldInCustomIndex { get; set; }

        public bool BooleanTransferedAsInteger { get; set; }

        public PacketSerializableAttribute(bool booleanTransferedAsInteger = false)
        {
            BooleanTransferedAsInteger = booleanTransferedAsInteger;
        }
    }
}
