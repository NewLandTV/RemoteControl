using System.Net;
using System.Net.Sockets;
using System.Text;
using UnityEngine;

public class Client : MonoBehaviour
{
    private const int SERVER_PORT = 7234;

    private Socket socket;

    private void OnApplicationQuit()
    {
        Disconnect();
    }

    public void ConnectToServer()
    {
        if (socket == null)
        {
            socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            socket.Connect(IPAddress.Parse("x.x.x.x"), SERVER_PORT);
        }
    }

    public void Disconnect()
    {
        if (socket.Connected)
        {
            socket.Close();

            socket = null;
        }
    }

    public void OnToggleValueChanged(bool isOn)
    {
        Send(isOn ? "T" : "F");
    }

    public void Send(string message)
    {
        socket.Send(Encoding.ASCII.GetBytes(message), SocketFlags.None);
    }
}
