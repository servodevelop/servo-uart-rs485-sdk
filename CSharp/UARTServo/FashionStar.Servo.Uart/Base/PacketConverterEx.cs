using BrightJade.Logging;
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace BrightJade
{
    public static class PacketConverterEx
    {
        private static Dictionary<Type, SortedDictionary<int, MemberInfo>> _typeMembersBuffer = new Dictionary<Type, SortedDictionary<int, MemberInfo>>();

        private static Dictionary<Type, int> _objectSizeBuffer = new Dictionary<Type, int>();

        private static SortedDictionary<int, MemberInfo> GetMembers(Type type)
        {
            SortedDictionary<int, MemberInfo> members = null;

            try
            {
                if (_typeMembersBuffer.ContainsKey(type))
                {
                    members = _typeMembersBuffer[type];
                }
                else
                {
                    members = new SortedDictionary<int, MemberInfo>();
                    foreach (MemberInfo member in type.GetMembers(BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic))
                    {
                        if (member.IsDefined(typeof(PacketFieldAttribute), true))
                        {
                            PacketFieldAttribute pfa = member.GetCustomAttributes(
                                typeof(PacketFieldAttribute), true)[0] as PacketFieldAttribute;
                            int index = pfa.Order;
                            while (members.ContainsKey(index))
                            {
                                ++index;
                            }
                            members.Add(index, member);
                        }
                    }

                    _typeMembersBuffer.Add(type, members);
                }
            }
            catch (Exception ex)
            {
                Logger.Error(ex);
            }

            return members;
        }

        private static int GetCountBy(object obj, string countBy)
        {
            string[] strArr = null;
            int adjust = 0;
            string countField = string.Empty;
            Type type = obj.GetType();
            if (countBy.Contains("+"))
            {
                strArr = countBy.Split(new char[] { '+' });
                countField = strArr[0];
                adjust = int.Parse(strArr[1]);
            }
            else if (countBy.Contains("-"))
            {
                strArr = countBy.Split(new char[] { '-' });
                countField = strArr[0];
                adjust = int.Parse(strArr[1]) * -1;
            }
            else
            {
                countField = countBy;
            }
            int count = int.Parse(type.GetField(countField).GetValue(obj).ToString()) + adjust;
            return count;
        }

        public static string ToString(object obj, int indent = 0)
        {
            StringBuilder sb = new StringBuilder();

            try
            {
                string indentTab = new string('\t', indent);
                string innerIndentTab = new string('\t', indent + 1);

                sb.Append(indentTab);
                sb.Append("Object");

                if (obj == null)
                {
                    sb.Append(": Empty!\n");
                }
                else if (GetSize(obj) > 102400)
                {

                    if (obj.GetType().IsDefined(typeof(PacketSerializableAttribute), true))
                    {
                        Type type = obj.GetType();
                        sb.Append(" ");
                        sb.Append(type.ToString());
                    }
                    sb.Append(": Size is more than 100 KB.\n");
                }
                else if (obj is ByteBuffer)
                {
                    ByteBuffer buffer = obj as ByteBuffer;
                    sb.Append("Binary data: ");
                    for (int i = 0; i < buffer.Length; ++i)
                    {
                        if (i % 10 == 0)
                        {
                            sb.Append("\n");
                        }
                        sb.Append(string.Format("{0:X} ", buffer[i]));
                    }
                    sb.Append("\n");
                }
                else if (obj is byte[])
                {
                    byte[] buffer = (byte[])obj;

                    sb.Append("Binary data: ");
                    for (int i = 0; i < buffer.Length; ++i)
                    {
                        if (i % 10 == 0)
                        {
                            sb.Append("\n");
                        }
                        sb.Append(string.Format("{0:X} ", buffer[i]));
                    }
                    sb.Append("\n");
                }
                else if (obj.GetType().IsDefined(typeof(PacketSerializableAttribute), true))
                {
                    Type type = obj.GetType();
                    sb.Append(" ");
                    sb.Append(type.ToString());
                    sb.Append(" Content: \n");

                    SortedDictionary<int, MemberInfo> members = GetMembers(type);

                    foreach (KeyValuePair<int, MemberInfo> pair in members)
                    {
                        sb.Append(innerIndentTab);

                        MemberInfo member = pair.Value;
                        PacketFieldAttribute pfa = member.GetCustomAttributes(
                            typeof(PacketFieldAttribute), true)[0] as PacketFieldAttribute;

                        object fieldValue = member.GetMemberValue(obj);
                        string typeString = member.GetMemberType().Name.Replace("[]", string.Empty);

                        if (member.GetMemberType().IsArray)
                        {
                            int arrayCount = (fieldValue == null) ? 0 : ((Array)fieldValue).Length;

                            Array vals = fieldValue as Array;

                            if (arrayCount == 0)
                            {
                                sb.Append(member.Name);
                                sb.Append(" = No elements in the array.");
                            }
                            for (int i = 0; i < arrayCount; ++i)
                            {
                                object val = vals.GetValue(i);

                                if (i != 0)
                                {
                                    sb.Append(innerIndentTab);
                                }
                                sb.Append(member.Name);
                                sb.Append("[");
                                sb.Append(i.ToString());
                                sb.Append("] = ");

                                if (vals == null || val == null)
                                {
                                    sb.Append("null");
                                }
                                else
                                {
                                    switch (typeString)
                                    {
                                        case "String":
                                            {
                                                sb.Append("'");
                                                sb.Append(val);
                                                sb.Append("'");
                                            }
                                            break;
                                        default:
                                            {
                                                if (pfa.IsCustomType)
                                                {
                                                    sb.Append("\n");
                                                    sb.Append(ToString(val, indent + 1));
                                                }
                                                else
                                                {
                                                    sb.Append(val.ToString());
                                                    if (i != arrayCount - 1)
                                                    {
                                                        sb.Append("\n");
                                                    }
                                                }
                                            }
                                            break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            sb.Append(member.Name);
                            sb.Append(" = ");

                            if (fieldValue == null)
                            {
                                sb.Append("null");
                            }
                            else
                            {
                                switch (typeString)
                                {
                                    case "String":
                                        {
                                            sb.Append("'");
                                            sb.Append(fieldValue);
                                            sb.Append("'");
                                        }
                                        break;
                                    default:
                                        {
                                            if (pfa.IsCustomType)
                                            {
                                                sb.Append("\n");
                                                sb.Append(ToString(fieldValue, indent + 1));
                                            }
                                            else
                                            {
                                                sb.Append(fieldValue.ToString());
                                            }
                                        }
                                        break;
                                }
                            }
                        }
                        sb.Append("\n");
                    }
                }
                else
                {
                    sb.Append("The object has no PacketSerializableAttribute tag.\n");
                }
            }
            catch (Exception ex)
            {
                Logger.Error(ex);
            }

            return sb.ToString();
        }

        public static TReturnType GetObject<TReturnType>(byte[] buffer)
        {
            return GetObject<TReturnType>(new ByteBuffer(buffer));
        }

        public static object GetObject(Type type, byte[] buffer)
        {
            return GetObject(type, new ByteBuffer(buffer));
        }

        public static TReturnType GetObject<TReturnType>(ByteBuffer buffer)
        {
            return (TReturnType)GetObject(typeof(TReturnType), buffer);
        }

        public static object GetObject(Type type, ByteBuffer buffer)
        {
            object obj = null;

            try
            {
                if (type != null && buffer != null && type.IsDefined(typeof(PacketSerializableAttribute), true))
                {
                    obj = type.GetConstructor(Type.EmptyTypes).Invoke(new object[] { });

                    PacketSerializableAttribute psa = type.GetCustomAttributes(
                        typeof(PacketSerializableAttribute), true)[0] as PacketSerializableAttribute;

                    SortedDictionary<int, MemberInfo> members = GetMembers(type);

                    foreach (KeyValuePair<int, MemberInfo> pair in members)
                    {
                        MemberInfo member = pair.Value;
                        PacketFieldAttribute pfa = member.GetCustomAttributes(
                            typeof(PacketFieldAttribute), true)[0] as PacketFieldAttribute;

                        Type elementType = Type.GetType(member.GetMemberType().AssemblyQualifiedName.Replace("[]", string.Empty));
                        string typeString = elementType.Name;

                        int fieldSize = 0;
                        if (pfa.SizeBy != string.Empty)
                        {
                            fieldSize = int.Parse(type.GetField(pfa.SizeBy).GetValue(obj).ToString());
                            if (fieldSize < 0)
                            {
                                fieldSize = 0;
                            }
                        }
                        else if (pfa.Size > 0)
                        {
                            fieldSize = pfa.Size;
                        }

                        if (member.GetMemberType().IsArray)
                        {
                            int arrayCount = 1;
                            if (pfa.CountBy != string.Empty)
                            {
                                arrayCount = GetCountBy(obj, pfa.CountBy);
                                if (arrayCount < 0)
                                {
                                    arrayCount = 0;
                                }
                            }
                            else if (pfa.Count > 0)
                            {
                                arrayCount = pfa.Count;
                            }

                            Array vals = Array.CreateInstance(elementType, arrayCount);

                            switch (typeString)
                            {
                                case "Boolean":
                                    {
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            if (psa.BooleanTransferedAsInteger)
                                            {
                                                vals.SetValue(buffer.ReadInt() == 0 ? false : true, i);
                                            }
                                            else
                                            {
                                                vals.SetValue(buffer.ReadBool(), i);
                                            }
                                        }
                                    }
                                    break;
                                case "Byte":
                                    {
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            vals.SetValue(buffer.ReadByte(), i);
                                        }
                                    }
                                    break;
                                case "Int16":
                                    {
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            vals.SetValue(buffer.ReadShort(), i);
                                        }
                                    }
                                    break;
                                case "UInt16":
                                    {
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            vals.SetValue(buffer.ReadUShort(), i);
                                        }
                                    }
                                    break;
                                case "Int32":
                                    {
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            vals.SetValue(buffer.ReadInt(), i);
                                        }
                                    }
                                    break;
                                case "UInt32":
                                    {
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            vals.SetValue(buffer.ReadUInt(), i);
                                        }
                                    }
                                    break;
                                case "Int64":
                                    {
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            vals.SetValue(buffer.ReadLong(), i);
                                        }
                                    }
                                    break;
                                case "UInt64":
                                    {
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            vals.SetValue(buffer.ReadULong(), i);
                                        }
                                    }
                                    break;
                                case "String":
                                    {
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            vals.SetValue(buffer.ReadString(fieldSize), i);
                                        }
                                    }
                                    break;
                                default:
                                    {
                                        if (pfa.IsCustomType)
                                        {
                                            for (int i = 0; i < arrayCount; ++i)
                                            {
                                                vals.SetValue(GetObject(elementType, buffer), i);
                                            }
                                        }
                                        else if (member.GetMemberType().IsEnum)
                                        {
                                            switch (Enum.GetUnderlyingType(elementType).Name)
                                            {
                                                case "Byte":
                                                    {
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            vals.SetValue(Enum.ToObject(elementType, buffer.ReadByte()), i);
                                                        }
                                                    }
                                                    break;
                                                case "Int16":
                                                    {
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            vals.SetValue(Enum.ToObject(elementType, buffer.ReadShort()), i);
                                                        }
                                                    }
                                                    break;
                                                case "UInt16":
                                                    {
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            vals.SetValue(Enum.ToObject(elementType, buffer.ReadUShort()), i);
                                                        }
                                                    }
                                                    break;
                                                case "Int32":
                                                    {
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            vals.SetValue(Enum.ToObject(elementType, buffer.ReadInt()), i);
                                                        }
                                                    }
                                                    break;
                                                case "UInt32":
                                                    {
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            vals.SetValue(Enum.ToObject(elementType, buffer.ReadUInt()), i);
                                                        }
                                                    }
                                                    break;
                                                case "Int64":
                                                    {
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            vals.SetValue(Enum.ToObject(elementType, buffer.ReadLong()), i);
                                                        }
                                                    }
                                                    break;
                                                case "UInt64":
                                                    {
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            vals.SetValue(Enum.ToObject(elementType, buffer.ReadULong()), i);
                                                        }
                                                    }
                                                    break;
                                            }
                                        }
                                    }
                                    break;
                            }
                            member.SetValue(obj, vals);
                        }
                        else
                        {
                            object val = null;
                            switch (typeString)
                            {
                                case "Boolean":
                                    {
                                        if (psa.BooleanTransferedAsInteger)
                                        {
                                            val = buffer.ReadInt() == 0 ? false : true;
                                        }
                                        else
                                        {
                                            val = buffer.ReadBool();
                                        }
                                    }
                                    break;
                                case "Byte":
                                    {
                                        val = buffer.ReadByte();
                                    }
                                    break;
                                case "Int16":
                                    {
                                        val = buffer.ReadShort();
                                    }
                                    break;
                                case "UInt16":
                                    {
                                        val = buffer.ReadUShort();
                                    }
                                    break;
                                case "Int32":
                                    {
                                        val = buffer.ReadInt();
                                    }
                                    break;
                                case "UInt32":
                                    {
                                        val = buffer.ReadUInt();
                                    }
                                    break;
                                case "Int64":
                                    {
                                        val = buffer.ReadLong();
                                    }
                                    break;
                                case "UInt64":
                                    {
                                        val = buffer.ReadULong();
                                    }
                                    break;
                                case "String":
                                    {
                                        val = buffer.ReadString(fieldSize);
                                    }
                                    break;
                                default:
                                    {
                                        if (pfa.IsCustomType)
                                        {
                                            val = GetObject(member.GetMemberType(), buffer);
                                        }
                                        else if (member.GetMemberType().IsEnum)
                                        {
                                            switch (Enum.GetUnderlyingType(elementType).Name)
                                            {
                                                case "Byte":
                                                    {
                                                        val = Enum.ToObject(elementType, buffer.ReadByte());
                                                    }
                                                    break;
                                                case "Int16":
                                                    {
                                                        val = Enum.ToObject(elementType, buffer.ReadShort());
                                                    }
                                                    break;
                                                case "UInt16":
                                                    {
                                                        val = Enum.ToObject(elementType, buffer.ReadUShort());
                                                    }
                                                    break;
                                                case "Int32":
                                                    {
                                                        val = Enum.ToObject(elementType, buffer.ReadInt());
                                                    }
                                                    break;
                                                case "UInt32":
                                                    {
                                                        val = Enum.ToObject(elementType, buffer.ReadUInt());
                                                    }
                                                    break;
                                                case "Int64":
                                                    {
                                                        val = Enum.ToObject(elementType, buffer.ReadLong());
                                                    }
                                                    break;
                                                case "UInt64":
                                                    {
                                                        val = Enum.ToObject(elementType, buffer.ReadULong());
                                                    }
                                                    break;
                                            }
                                        }
                                    }
                                    break;
                            }
                            member.SetValue(obj, val);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Logger.Error(ex);
            }

            return obj;
        }

        public static int GetSize(object obj)
        {
            int size = 0;

            try
            {
                if (obj != null)
                {
                    Type type = obj.GetType();

                    if (_objectSizeBuffer.ContainsKey(obj.GetType()))
                    {
                        size = _objectSizeBuffer[type];
                    }
                    else
                    {
                        if (type.IsDefined(typeof(PacketSerializableAttribute), true))
                        {
                            PacketSerializableAttribute psa = type.GetCustomAttributes(
                                typeof(PacketSerializableAttribute), true)[0] as PacketSerializableAttribute;

                            SortedDictionary<int, MemberInfo> members = GetMembers(type);

                            bool isDynamicSize = false;
                            foreach (KeyValuePair<int, MemberInfo> pair in members)
                            {
                                MemberInfo field = pair.Value;
                                PacketFieldAttribute pfa = field.GetCustomAttributes(
                                    typeof(PacketFieldAttribute), true)[0] as PacketFieldAttribute;

                                object fieldValue = field.GetMemberValue(obj);
                                Type elementType = Type.GetType(field.GetMemberType().AssemblyQualifiedName.Replace("[]", string.Empty));
                                string typeString = elementType.Name;

                                if (pfa.IsDynamicSize)
                                {
                                    isDynamicSize = true;
                                }

                                int fieldSize = 0;
                                if (pfa.SizeBy != string.Empty)
                                {
                                    isDynamicSize = true;
                                    fieldSize = int.Parse(type.GetField(pfa.SizeBy).GetValue(obj).ToString());
                                    if (fieldSize < 0)
                                    {
                                        fieldSize = 0;
                                    }
                                }
                                else if (pfa.Size > 0)
                                {
                                    fieldSize = pfa.Size;
                                }

                                int arrayCount = 1;
                                if (field.GetMemberType().IsArray)
                                {
                                    if (pfa.CountBy != string.Empty)
                                    {
                                        isDynamicSize = true;
                                        arrayCount = GetCountBy(obj, pfa.CountBy);
                                        if (arrayCount < 0)
                                        {
                                            arrayCount = 0;
                                        }
                                    }
                                    else if (pfa.Count > 0)
                                    {
                                        arrayCount = pfa.Count;
                                    }
                                    else
                                    {
                                        if (fieldValue != null)
                                        {
                                            arrayCount = ((Array)fieldValue).Length;
                                        }
                                    }
                                }

                                switch (typeString)
                                {
                                    case "Boolean":
                                        {
                                            size += arrayCount * (psa.BooleanTransferedAsInteger ? 4 : 1);
                                        }
                                        break;
                                    case "Byte":
                                        {
                                            size += arrayCount;
                                        }
                                        break;
                                    case "Int16":
                                    case "UInt16":
                                        {
                                            size += arrayCount * 2;
                                        }
                                        break;
                                    case "Int32":
                                    case "UInt32":
                                        {
                                            size += arrayCount * 4;
                                        }
                                        break;
                                    case "Int64":
                                    case "UInt64":
                                        {
                                            size += arrayCount * 8;
                                        }
                                        break;
                                    case "String":
                                        {
                                            size += arrayCount * fieldSize * 2;
                                        }
                                        break;
                                    default:
                                        {
                                            if (pfa.IsCustomType)
                                            {
                                                if (field.GetMemberType().IsArray)
                                                {
                                                    object[] vals = (fieldValue == null) ? new object[arrayCount] : (object[])fieldValue; ;

                                                    object val = null;
                                                    if (arrayCount == 0 || vals[0] == null)
                                                    {
                                                        val = elementType.GetConstructor(Type.EmptyTypes).Invoke(new object[] { });
                                                    }
                                                    else
                                                    {
                                                        val = vals[0];
                                                    }

                                                    size += GetSize(val) * arrayCount;
                                                }
                                                else
                                                {
                                                    object val = null;
                                                    if (fieldValue == null)
                                                    {
                                                        val = elementType.GetConstructor(Type.EmptyTypes).Invoke(new object[] { });
                                                    }
                                                    else
                                                    {
                                                        val = fieldValue;
                                                    }

                                                    size += GetSize(val);
                                                }
                                            }
                                            else if (field.GetMemberType().IsEnum)
                                            {
                                                switch (Enum.GetUnderlyingType(elementType).Name)
                                                {
                                                    case "Byte":
                                                        {
                                                            size += arrayCount;
                                                        }
                                                        break;
                                                    case "Int16":
                                                    case "UInt16":
                                                        {
                                                            size += arrayCount * 2;
                                                        }
                                                        break;
                                                    case "Int32":
                                                    case "UInt32":
                                                        {
                                                            size += arrayCount * 4;
                                                        }
                                                        break;
                                                    case "Int64":
                                                    case "UInt64":
                                                        {
                                                            size += arrayCount * 8;
                                                        }
                                                        break;
                                                }
                                            }
                                        }
                                        break;
                                }
                            }

                            if (!isDynamicSize)
                            {
                                _objectSizeBuffer.Add(type, size);
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Logger.Error(ex);
            }

            return size;
        }

        public static byte[] GetBytes(object obj)
        {
            ByteBuffer buffer = new ByteBuffer(GetSize(obj));

            try
            {
                if (obj != null && obj.GetType().IsDefined(typeof(PacketSerializableAttribute), true))
                {
                    Type type = obj.GetType();

                    PacketSerializableAttribute psa = type.GetCustomAttributes(
                        typeof(PacketSerializableAttribute), true)[0] as PacketSerializableAttribute;

                    SortedDictionary<int, MemberInfo> members = GetMembers(type);

                    foreach (KeyValuePair<int, MemberInfo> pair in members)
                    {
                        MemberInfo member = pair.Value;
                        PacketFieldAttribute pfa = member.GetCustomAttributes(
                            typeof(PacketFieldAttribute), true)[0] as PacketFieldAttribute;


                        object fieldValue = member.GetMemberValue(obj);
                        Type elementType = Type.GetType(member.GetMemberType().AssemblyQualifiedName.Replace("[]", string.Empty));
                        string typeString = elementType.Name;

                        int fieldSize = 0;
                        if (pfa.SizeBy != string.Empty)
                        {
                            fieldSize = int.Parse(type.GetField(pfa.SizeBy).GetValue(obj).ToString());
                            if (fieldSize < 0)
                            {
                                fieldSize = 0;
                            }
                        }
                        else if (pfa.Size > 0)
                        {
                            fieldSize = pfa.Size;
                        }

                        if (member.GetMemberType().IsArray)
                        {
                            int arrayCount = 1;
                            if (pfa.CountBy != string.Empty)
                            {
                                arrayCount = GetCountBy(obj, pfa.CountBy);
                                if (arrayCount < 0)
                                {
                                    arrayCount = 0;
                                }
                            }
                            else if (pfa.Count > 0)
                            {
                                arrayCount = pfa.Count;
                            }
                            else
                            {
                                if (fieldValue != null)
                                {
                                    arrayCount = ((Array)fieldValue).Length;
                                }
                            }

                            switch (typeString)
                            {
                                case "Boolean":
                                    {
                                        bool[] vals = (bool[])fieldValue;
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            if (vals == null || i >= vals.Length)
                                            {
                                                if (psa.BooleanTransferedAsInteger)
                                                {
                                                    buffer.WriteInt(0);
                                                }
                                                else
                                                {
                                                    buffer.WriteBool(false);
                                                }
                                            }
                                            else
                                            {
                                                if (psa.BooleanTransferedAsInteger)
                                                {
                                                    buffer.WriteInt(vals[i] ? 1 : 0);
                                                }
                                                else
                                                {
                                                    buffer.WriteBool(vals[i]);
                                                }
                                            }
                                        }
                                    }
                                    break;
                                case "Byte":
                                    {
                                        byte[] vals = (byte[])fieldValue;
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            if (vals == null || i >= vals.Length)
                                            {
                                                buffer.WriteByte(0);
                                            }
                                            else
                                            {
                                                buffer.WriteByte(vals[i]);
                                            }
                                        }
                                    }
                                    break;
                                case "Int16":
                                    {
                                        short[] vals = (short[])fieldValue;
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            if (vals == null || i >= vals.Length)
                                            {
                                                buffer.WriteShort(0);
                                            }
                                            else
                                            {
                                                buffer.WriteShort(vals[i]);
                                            }
                                        }
                                    }
                                    break;
                                case "UInt16":
                                    {
                                        ushort[] vals = (ushort[])fieldValue;
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            if (vals == null || i >= vals.Length)
                                            {
                                                buffer.WriteUShort(0);
                                            }
                                            else
                                            {
                                                buffer.WriteUShort(vals[i]);
                                            }
                                        }
                                    }
                                    break;
                                case "Int32":
                                    {
                                        int[] vals = (int[])fieldValue;
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            if (vals == null || i >= vals.Length)
                                            {
                                                buffer.WriteInt(0);
                                            }
                                            else
                                            {
                                                buffer.WriteInt(vals[i]);
                                            }
                                        }
                                    }
                                    break;
                                case "UInt32":
                                    {
                                        uint[] vals = (uint[])fieldValue;
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            if (vals == null || i >= vals.Length)
                                            {
                                                buffer.WriteUInt(0);
                                            }
                                            else
                                            {
                                                buffer.WriteUInt(vals[i]);
                                            }
                                        }
                                    }
                                    break;
                                case "Int64":
                                    {
                                        long[] vals = (long[])fieldValue;
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            if (vals == null || i >= vals.Length)
                                            {
                                                buffer.WriteLong(0);
                                            }
                                            else
                                            {
                                                buffer.WriteLong(vals[i]);
                                            }
                                        }
                                    }
                                    break;
                                case "UInt64":
                                    {
                                        ulong[] vals = (ulong[])fieldValue;
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            if (vals == null || i >= vals.Length)
                                            {
                                                buffer.WriteULong(0);
                                            }
                                            else
                                            {
                                                buffer.WriteULong(vals[i]);
                                            }
                                        }
                                    }
                                    break;
                                case "String":
                                    {
                                        string[] vals = (fieldValue == null) ? new string[arrayCount] : (string[])fieldValue;
                                        for (int i = 0; i < arrayCount; ++i)
                                        {
                                            string val = (vals[i] == null) ? string.Empty : vals[i];
                                            if (val.Length > fieldSize)
                                            {
                                                val = val.Substring(0, fieldSize);
                                            }
                                            else if (val.Length < fieldSize)
                                            {
                                                val = val.PadRight(fieldSize, '\0');
                                            }

                                            buffer.WriteString(val);
                                        }
                                    }
                                    break;
                                default:
                                    {
                                        if (pfa.IsCustomType)
                                        {
                                            object[] vals = (fieldValue == null) ? new object[arrayCount] : (object[])fieldValue; ;

                                            for (int i = 0; i < arrayCount; ++i)
                                            {
                                                object val = null;
                                                if (vals[i] == null)
                                                {
                                                    val = elementType.GetConstructor(Type.EmptyTypes).Invoke(new object[] { });
                                                }
                                                else
                                                {
                                                    val = vals[i];
                                                }

                                                byte[] fieldBytes = GetBytes(val);
                                                buffer.WriteBytes(fieldBytes, 0, fieldBytes.Length);
                                            }
                                        }
                                        else if (member.GetMemberType().IsEnum)
                                        {
                                            switch (Enum.GetUnderlyingType(elementType).Name)
                                            {
                                                case "Byte":
                                                    {
                                                        byte[] vals = (byte[])fieldValue;
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            if (vals == null || i >= vals.Length)
                                                            {
                                                                buffer.WriteByte(0);
                                                            }
                                                            else
                                                            {
                                                                buffer.WriteByte(vals[i]);
                                                            }
                                                        }
                                                    }
                                                    break;
                                                case "Int16":
                                                    {
                                                        short[] vals = (short[])fieldValue;
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            if (vals == null || i >= vals.Length)
                                                            {
                                                                buffer.WriteShort(0);
                                                            }
                                                            else
                                                            {
                                                                buffer.WriteShort(vals[i]);
                                                            }
                                                        }
                                                    }
                                                    break;
                                                case "UInt16":
                                                    {
                                                        ushort[] vals = (ushort[])fieldValue;
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            if (vals == null || i >= vals.Length)
                                                            {
                                                                buffer.WriteUShort(0);
                                                            }
                                                            else
                                                            {
                                                                buffer.WriteUShort(vals[i]);
                                                            }
                                                        }
                                                    }
                                                    break;
                                                case "Int32":
                                                    {
                                                        int[] vals = (int[])fieldValue;
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            if (vals == null || i >= vals.Length)
                                                            {
                                                                buffer.WriteInt(0);
                                                            }
                                                            else
                                                            {
                                                                buffer.WriteInt(vals[i]);
                                                            }
                                                        }
                                                    }
                                                    break;
                                                case "UInt32":
                                                    {
                                                        uint[] vals = (uint[])fieldValue;
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            if (vals == null || i >= vals.Length)
                                                            {
                                                                buffer.WriteUInt(0);
                                                            }
                                                            else
                                                            {
                                                                buffer.WriteUInt(vals[i]);
                                                            }
                                                        }
                                                    }
                                                    break;
                                                case "Int64":
                                                    {
                                                        long[] vals = (long[])fieldValue;
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            if (vals == null || i >= vals.Length)
                                                            {
                                                                buffer.WriteLong(0);
                                                            }
                                                            else
                                                            {
                                                                buffer.WriteLong(vals[i]);
                                                            }
                                                        }
                                                    }
                                                    break;
                                                case "UInt64":
                                                    {
                                                        ulong[] vals = (ulong[])fieldValue;
                                                        for (int i = 0; i < arrayCount; ++i)
                                                        {
                                                            if (vals == null || i >= vals.Length)
                                                            {
                                                                buffer.WriteULong(0);
                                                            }
                                                            else
                                                            {
                                                                buffer.WriteULong(vals[i]);
                                                            }
                                                        }
                                                    }
                                                    break;
                                            }
                                        }
                                    }
                                    break;
                            }
                        }
                        else
                        {
                            switch (typeString)
                            {
                                case "Boolean":
                                    {
                                        bool val = (bool)fieldValue;
                                        if (psa.BooleanTransferedAsInteger)
                                        {
                                            buffer.WriteInt(val ? 1 : 0);
                                        }
                                        else
                                        {
                                            buffer.WriteBool(val);
                                        }
                                    }
                                    break;
                                case "Byte":
                                    {
                                        byte val = (byte)fieldValue;
                                        buffer.WriteByte(val);
                                    }
                                    break;
                                case "Int16":
                                    {
                                        short val = (short)fieldValue;
                                        buffer.WriteShort(val);
                                    }
                                    break;
                                case "UInt16":
                                    {
                                        ushort val = (ushort)fieldValue;
                                        buffer.WriteUShort(val);
                                    }
                                    break;
                                case "Int32":
                                    {
                                        int val = (int)fieldValue;
                                        buffer.WriteInt(val);
                                    }
                                    break;
                                case "UInt32":
                                    {
                                        uint val = (uint)fieldValue;
                                        buffer.WriteUInt(val);
                                    }
                                    break;
                                case "Int64":
                                    {
                                        long val = (long)fieldValue;
                                        buffer.WriteLong(val);
                                    }
                                    break;
                                case "UInt64":
                                    {
                                        ulong val = (ulong)fieldValue;
                                        buffer.WriteULong(val);
                                    }
                                    break;
                                case "String":
                                    {
                                        string val = (fieldValue == null) ? string.Empty : (string)fieldValue;
                                        if (val.Length > fieldSize)
                                        {
                                            val = val.Substring(0, fieldSize);
                                        }
                                        else if (val.Length < fieldSize)
                                        {
                                            val = val.PadRight(fieldSize, '\0');
                                        }

                                        buffer.WriteString(val);
                                    }
                                    break;
                                default:
                                    {
                                        if (pfa.IsCustomType)
                                        {
                                            object val = null;
                                            if (fieldValue == null)
                                            {
                                                val = elementType.GetConstructor(Type.EmptyTypes).Invoke(new object[] { });
                                            }
                                            else
                                            {
                                                val = fieldValue;
                                            }

                                            byte[] fieldBytes = GetBytes(val);
                                            buffer.WriteBytes(fieldBytes, 0, fieldBytes.Length);
                                        }
                                        else if (member.GetMemberType().IsEnum)
                                        {
                                            switch (Enum.GetUnderlyingType(elementType).Name)
                                            {
                                                case "Byte":
                                                    {
                                                        byte val = (byte)fieldValue;
                                                        buffer.WriteByte(val);
                                                    }
                                                    break;
                                                case "Int16":
                                                    {
                                                        short val = (short)fieldValue;
                                                        buffer.WriteShort(val);
                                                    }
                                                    break;
                                                case "UInt16":
                                                    {
                                                        ushort val = (ushort)fieldValue;
                                                        buffer.WriteUShort(val);
                                                    }
                                                    break;
                                                case "Int32":
                                                    {
                                                        int val = (int)fieldValue;
                                                        buffer.WriteInt(val);
                                                    }
                                                    break;
                                                case "UInt32":
                                                    {
                                                        uint val = (uint)fieldValue;
                                                        buffer.WriteUInt(val);
                                                    }
                                                    break;
                                                case "Int64":
                                                    {
                                                        long val = (long)fieldValue;
                                                        buffer.WriteLong(val);
                                                    }
                                                    break;
                                                case "UInt64":
                                                    {
                                                        ulong val = (ulong)fieldValue;
                                                        buffer.WriteULong(val);
                                                    }
                                                    break;
                                            }
                                        }
                                    }
                                    break;
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Logger.Error(ex);
            }

            return buffer.GetBuffer();
        }
    }
}
