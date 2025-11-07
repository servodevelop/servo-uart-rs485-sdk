namespace FashionStar.Servo.Uart
{
    public interface ISyncCommandInfo
    {
        int Count { get; }
        int PacketContentLength { get; }
        byte PacketID { get; }

        byte[] GetBytes();
    }
}