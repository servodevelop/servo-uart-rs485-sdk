#if VS_IDE

using System;

namespace BrightJade.Logging
{
    public class DebugLogProvider :ILogProvider
    {
        public bool Enabled { get; set; }

        public LogLevel ShowLevel { get; set; }

        public void Debug(string message, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Debug)
            {
                System.Diagnostics.Debug.Write(message);
            }
        }

        public void Error(string message, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Error)
            {
                System.Diagnostics.Debug.Write(message);
            }
        }

        public void Error(Exception ex, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Error)
            {
                System.Diagnostics.Debug.Print("{0}\n{1}", ex.Message, ex.StackTrace);
            }
        }

        public void Fatal(string message, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Fatal)
            {
                System.Diagnostics.Debug.Write(message);
            }
        }

        public void Fatal(Exception ex, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Fatal)
            {
                System.Diagnostics.Debug.Print("{0}\n{1}", ex.Message, ex.StackTrace);
            }
        }

        public void Info(string message, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Info)
            {
                System.Diagnostics.Debug.Write(message);
            }
        }

        public void Log(LogLevel level, object obj, int group)
        {
            string message;
            if (obj is string)
            {
                message = obj as string;
            }
            else
            {
                message = PacketConverterEx.ToString(obj);
            }

            switch (level)
            {
                case LogLevel.Debug:
                    Debug(message, group);
                    break;
                case LogLevel.Info:
                    Info(message, group);
                    break;
                case LogLevel.Warning:
                    Warning(message, group);
                    break;
                case LogLevel.Error:
                    Error(message, group);
                    break;
                case LogLevel.Fatal:
                    Fatal(message, group);
                    break;
                default:
                    Info(message, group);
                    break;
            }
        }

        public void Warning(string message, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Warning)
            {
                System.Diagnostics.Debug.Write(message);
            }
        }
    }
}
#endif