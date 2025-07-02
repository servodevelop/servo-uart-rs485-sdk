using System;
using System.IO;
using System.Text;
using System.Timers;

namespace BrightJade.Logging
{
    public class FileLogProvider : ILogProvider
    {
        private Timer _timer;

        private bool _writeFileFinished = true;

        private int _currentBufferIndex = 0;

        private StringBuilder[] _stringBuffers;

        public string LogFileName { get; set; }

        public string LogFormat { get; set; }

        public bool Enabled
        {
            get { return _timer.Enabled; }
            set { _timer.Enabled = value; }
        }

        public LogLevel ShowLevel { get; set; }

        public int LogInterval
        {
            get { return (int)_timer.Interval; }
            set { _timer.Interval = value; }
        }

        public FileLogProvider()
        {
            LogFileName = "log.txt";
            Initialize();
        }

        public FileLogProvider(string logFileName)
        {
            LogFileName = logFileName;
            Initialize();
        }

        ~FileLogProvider()
        {
            WriteMessageToFile();
        }

        private void Initialize()
        {
            LogFormat = "{0}\t{1}\r\n{2}";
            _stringBuffers = new StringBuilder[2];
            _stringBuffers[0] = new StringBuilder();
            _stringBuffers[1] = new StringBuilder();

            _timer = new Timer(3000);
            _timer.Elapsed += new ElapsedEventHandler(OnTimerElapsed);
        }

        private void WriteMessageToFile()
        {
            if (_stringBuffers[_currentBufferIndex].Length > 0 && _writeFileFinished)
            {
                _writeFileFinished = false;
                _currentBufferIndex = (_currentBufferIndex == 0) ? 1 : 0;
                int writtenIndex = (_currentBufferIndex == 0) ? 1 : 0;

                StringBuilder writtenBuffer = _stringBuffers[writtenIndex];
                FileStream stream = new FileStream(LogFileName, FileMode.Append, FileAccess.Write, FileShare.ReadWrite, 1024, true);
                byte[] bytes = Encoding.Default.GetBytes(writtenBuffer.ToString());

                writtenBuffer.Length = 0;
                stream.BeginWrite(bytes, 0, bytes.Length, new AsyncCallback(EndWriteMessageToFile), stream);
                stream.Flush();
            }
        }

        private void OnTimerElapsed(object sender, ElapsedEventArgs e)
        {
            WriteMessageToFile();
        }

        private void EndWriteMessageToFile(IAsyncResult result)
        {
            FileStream stream = (FileStream)result.AsyncState;
            stream.EndWrite(result);
            stream.Close();
            _writeFileFinished = true;
        }

        protected void SendMessage(LogLevel level, string message, int group)
        {
            StringBuilder sb = _stringBuffers[_currentBufferIndex];
            sb.AppendLine(string.Format(LogFormat,
                level,
                DateTime.Now,
                message)
            );
        }

        public void Debug(string message, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Debug)
            {
                SendMessage(LogLevel.Debug, message, group);
            }
        }

        public void Error(string message, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Error)
            {
                SendMessage(LogLevel.Error, message, group);
            }
        }

        public void Error(Exception ex, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Error)
            {
                SendMessage(LogLevel.Error, string.Format("{0}\n{1}", ex.Message, ex.StackTrace), group);
            }
        }

        public void Fatal(string message, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Fatal)
            {
                SendMessage(LogLevel.Fatal, message, group);
            }
        }

        public void Fatal(Exception ex, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Fatal)
            {
                SendMessage(LogLevel.Fatal, string.Format("{0}\n{1}", ex.Message, ex.StackTrace), group);
            }
        }

        public void Info(string message, int group)
        {
            if (Enabled && ShowLevel >= LogLevel.Info)
            {
                SendMessage(LogLevel.Info, message, group);
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
                SendMessage(LogLevel.Warning, message, group);
            }
        }
    }
}
