using System;
using System.Collections.Generic;

namespace BrightJade.Logging
{
    public static class Logger
    {
        private static Dictionary<string, ILogProvider> _logProviderMap = new Dictionary<string, ILogProvider>();

        public static void SetAllEnable(bool enable)
        {
            foreach (KeyValuePair<string, ILogProvider> item in _logProviderMap)
            {
                item.Value.Enabled = enable;
            }
        }

        public static void SetAllLogLevel(LogLevel level)
        {
            foreach (KeyValuePair<string, ILogProvider> item in _logProviderMap)
            {
                item.Value.ShowLevel = level;
            }
        }

        public static ILogProvider GetLogProvider(string index)
        {
            if (_logProviderMap.ContainsKey(index))
            {
                return _logProviderMap[index];
            }
            else
            {
                return null;
            }
        }

        public static void AddLogProvider(string providerName, ILogProvider provider)
        {
            RemoveProvider(providerName);
            _logProviderMap.Add(providerName, provider);
        }

        public static void RemoveProvider(string providerName)
        {
            if (_logProviderMap.ContainsKey(providerName))
            {
                _logProviderMap.Remove(providerName);
            }
        }

        public static void Debug(string message)
        {
            Debug(message, 0);
        }

        public static void Debug(string message, int group)
        {
            foreach (KeyValuePair<string, ILogProvider> item in _logProviderMap)
            {
                item.Value.Debug(message, group);
            }
        }

        public static void Error(string message)
        {
            Error(message, 0);
        }

        public static void Error(string message, int group)
        {
            foreach (KeyValuePair<string, ILogProvider> item in _logProviderMap)
            {
                item.Value.Error(message, group);
            }
        }

        public static void Error(Exception ex)
        {
            Error(ex, 0);
        }

        public static void Error(Exception ex, int group)
        {
            foreach (KeyValuePair<string, ILogProvider> item in _logProviderMap)
            {
                item.Value.Error(ex, group);
            }
        }

        public static void Fatal(string message)
        {
            Fatal(message, 0);
        }

        public static void Fatal(string message, int group)
        {
            foreach (KeyValuePair<string, ILogProvider> item in _logProviderMap)
            {
                item.Value.Fatal(message, group);
            }
        }

        public static void Fatal(Exception ex)
        {
            Fatal(ex, 0);
        }

        public static void Fatal(Exception ex, int group)
        {
            foreach (KeyValuePair<string, ILogProvider> item in _logProviderMap)
            {
                item.Value.Fatal(ex, group);
            }
        }

        public static void Info(string message)
        {
            Info(message, 0);
        }

        public static void Info(string message, int group)
        {
            foreach (KeyValuePair<string, ILogProvider> item in _logProviderMap)
            {
                item.Value.Info(message, group);
            }
        }

        public static void Log(LogLevel level, string message)
        {
            Log(level, message, 0);
        }

        public static void Log(LogLevel level, object obj, int group)
        {
            foreach (KeyValuePair<string, ILogProvider> item in _logProviderMap)
            {
                item.Value.Log(level, obj, group);
            }
        }

        public static void Warning(string message)
        {
            Warning(message, 0);
        }

        public static void Warning(string message, int group)
        {
            foreach (KeyValuePair<string, ILogProvider> item in _logProviderMap)
            {
                item.Value.Warning(message, group);
            }
        }
    }
}
