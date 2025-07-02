using System;

namespace BrightJade.Logging
{
    public interface ILogProvider
    {
        bool Enabled { get; set; }

        LogLevel ShowLevel { get; set; }

        void Debug(string message, int group);

        void Error(string message, int group);

        void Error(Exception ex, int group);

        void Fatal(string message, int group);

        void Fatal(Exception ex, int group);

        void Info(string message, int group);

        void Log(LogLevel level, object obj, int group);

        void Warning(string message, int group);
    }
}
