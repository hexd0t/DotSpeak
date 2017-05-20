using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DotSpeak
{
    public struct ServerConnectionHandler
    {
        internal readonly UInt64 ID;

        internal ServerConnectionHandler(UInt64 id)
        {
            ID = id;
        }

        public override bool Equals(object obj)
        {
            return obj is ServerConnectionHandler &&
                   ((ServerConnectionHandler) obj).ID == ID;
        }

        public override int GetHashCode()
        {
            return ID.GetHashCode();
        }

        public override string ToString()
        {
            return "<ServerConnection "+ID.ToString("X")+">";
        }
    }
    public struct AnyID
    {
        internal readonly UInt16 ID;

        internal AnyID(UInt16 id)
        {
            ID = id;
        }

        public override bool Equals(object obj)
        {
            return obj is ServerConnectionHandler &&
                   ((ServerConnectionHandler)obj).ID == ID;
        }

        public override int GetHashCode()
        {
            return ID.GetHashCode();
        }

        public override string ToString()
        {
            return "<AnyID " + ID.ToString("X") + ">";
        }
    }
}
