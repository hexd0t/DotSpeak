using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DotSpeak;

namespace Plugin
{
    public class Plugin : PluginBase, IDisposable
    {
        public static string Name = "TSWhiteboard";
        public static string Author = "hexd0t";
        public static string Version = "1.0";
        public static string Desc = "Adds shared whiteboard capabilities to TeamSpeak3 Channels";

        public static string CommandKeyword = "whiteboard";
        public static SettingDialogMode PluginSettingDialogMode = SettingDialogMode.NewThread;

        public Plugin()
        {
            Application.EnableVisualStyles();

            this.ProcessCommand += Plugin_ProcessCommand;
            PluginCommand += OnPluginCommand;
        }

        private void OnPluginCommand(PluginCommandEventArgs args)
        {
            MessageBox.Show(args.PluginCommand);
        }

        private void Plugin_ProcessCommand(ProcessCommandEventArgs args)
        {
            TS3Functions.SendPluginCommand(TS3Functions.GetCurrentServerConnectionHandler(), args.Command, PluginTargetMode.CurrentChannel, 
                new List<AnyID>());
            args.Handled = true;
        }

        public void Dispose()
        {
            MessageBox.Show("Bye!");
        }
    }
}
