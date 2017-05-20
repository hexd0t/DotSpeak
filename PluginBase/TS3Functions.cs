using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace DotSpeak
{
    internal static class TS3Interface
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern UInt64[] GetServerConnectionHandlerList();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern UInt64 GetCurrentServerConnectionHandler();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SendPluginCommand(UInt64 serverConnectionHandler, string command,
            Int32 targetMode, UInt16[] targetIDs);
    }

    public static class TS3Functions
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static IEnumerable<ServerConnectionHandler> GetServerConnectionHandlerList()
        {
            var source = TS3Interface.GetServerConnectionHandlerList();
            return source.Select((id) => new ServerConnectionHandler(id));
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static ServerConnectionHandler GetCurrentServerConnectionHandler()
        {
            return new ServerConnectionHandler(TS3Interface.GetCurrentServerConnectionHandler());
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void SendPluginCommand(ServerConnectionHandler serverConnectionHandler, string command,
            PluginTargetMode targetMode, ICollection<AnyID> targetIDs = null)
        {
            if (command == null)
                throw new ArgumentNullException(nameof(command));

            UInt16[] unwrappedIDs = null;
            if (targetIDs != null)
            {
                unwrappedIDs = new UInt16[targetIDs.Count + 1];
                int i = 0;
                foreach (var targetID in targetIDs)
                {
                    unwrappedIDs[i] = targetID.ID;
                    ++i;
                }
                //Terminate array as TS3 requires it, so the managed array data can be used directly
                unwrappedIDs[i] = 0; 
            }
            TS3Interface.SendPluginCommand(serverConnectionHandler.ID, command, (int)targetMode, unwrappedIDs);
        }
    }
}
