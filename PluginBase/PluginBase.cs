using System;
using System.Reflection;

namespace DotSpeak
{
    public class PluginBase
    {
        public enum SettingDialogMode : int { None = 0, NewThread, QTThread }

        public static readonly UInt32 DotSpeakAPIVersion = 1;

        public PluginBase()
        {
        }

        //======================================================================= Events
        public class ProcessCommandEventArgs : EventArgs {
            public bool Handled = false;
            public readonly ServerConnectionHandler ServerConnectionHandler;
            public readonly string Command;

            internal ProcessCommandEventArgs(ServerConnectionHandler handler, string command)
            {
                ServerConnectionHandler = handler;
                Command = command;
            }
        }
        public delegate void ProcessCommandEventHandler(ProcessCommandEventArgs args);
        /// <summary>
        /// Event called if User typed a command for this Plugin into a chat window.
        /// <remarks>This requires CommandKeyword to be specified!</remarks>
        /// </summary>
        public event ProcessCommandEventHandler ProcessCommand;

        public class ServerErrorEventArgs : EventArgs {
            public bool Handled = false;
            public readonly ServerConnectionHandler ServerConnectionHandler;
            public readonly TS3Error Error;
            public readonly string Message;
            public readonly string ExtraMessage;

            internal ServerErrorEventArgs(ServerConnectionHandler handler, TS3Error err, string msg, string extraMsg)
            {
                ServerConnectionHandler = handler;
                Error = err;
                Message = msg;
                ExtraMessage = extraMsg;
            }
        }
        public delegate void ServerErrorEventHandler(ServerErrorEventArgs args);
        public event ServerErrorEventHandler ServerError;

        public class PluginCommandEventArgs : EventArgs
        {
            public readonly ServerConnectionHandler ServerConnectionHandler;
            public readonly string PluginCommand;

            public PluginCommandEventArgs(ServerConnectionHandler serverConnectionHandler, string pluginCommand)
            {
                ServerConnectionHandler = serverConnectionHandler;
                PluginCommand = pluginCommand;
            }
        }
        public delegate void PluginCommandEventHandler(PluginCommandEventArgs args);
        public event PluginCommandEventHandler PluginCommand;

        //======================================================================= Event entry points for unmanaged wrapper
        // ReSharper disable UnusedMember.Local
        private bool OnProcessCommand(UInt64 serverConnectionHandlerID, string command)
        {
            if (ProcessCommand != null)
            {
                var args = new ProcessCommandEventArgs(new ServerConnectionHandler(serverConnectionHandlerID), command);
                ProcessCommand.Invoke(args);
                return args.Handled;
            }
            return false;
        }

        private bool OnServerError(UInt64 serverConnectionHandlerID, string msg, UInt32 error, string extraMsg)
        {
            if (ServerError != null)
            {
                var args = new ServerErrorEventArgs(new ServerConnectionHandler(serverConnectionHandlerID),
                    (TS3Error) error, msg, extraMsg);
                ServerError.Invoke(args);
                return args.Handled;
            }
            return false;
        }

        private void OnPluginCommand(UInt64 serverConnectionHandlerID, string pluginCommand)
        {
            if (PluginCommand != null)
            {
                var args = new PluginCommandEventArgs(new ServerConnectionHandler(serverConnectionHandlerID), pluginCommand);
                PluginCommand.Invoke(args);
            }
        }
        // ReSharper restore UnusedMember.Local
    }
}
