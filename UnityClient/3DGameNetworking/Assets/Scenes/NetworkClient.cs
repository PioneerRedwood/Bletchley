using UnityEngine;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Net;
using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using UnityEngine.UI;

#region MS C# Docs Asynchronous Client

public class StateObject
{
    public Socket workSocket = null;

    public const int BufferSize = 256;

    public byte[] buffer = new byte[BufferSize];

    public StringBuilder sb = new StringBuilder();
}

public class AsynchronousClient
{
    private const int port = 9000;

    private static ManualResetEvent connectDone = new ManualResetEvent(false);

    private static ManualResetEvent sendDone = new ManualResetEvent(false);

    private static ManualResetEvent receiveDone = new ManualResetEvent(false);

    private static string response = string.Empty;

    public static void StartClient()
    {
        try
        {
            //IPHostEntry ipHostInfo = Dns.GetHostEntry("127.0.0.1");
            //IPAddress ipAddress = ipHostInfo.AddressList[0];
            IPAddress ipAddress = IPAddress.Parse("127.0.0.1");
            IPEndPoint remoteEP = new IPEndPoint(ipAddress, port);

            Socket client = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            client.BeginConnect(remoteEP, new AsyncCallback(ConnectCallback), client);
            connectDone.WaitOne();

            Send(client, "This is a test<EOF>");
            sendDone.WaitOne();

            Receive(client);
            receiveDone.WaitOne();

            Debug.Log("Response received: " + response);

            client.Shutdown(SocketShutdown.Both);
            client.Close();
        }
        catch(Exception e)
        {
            Debug.Log(e);
        }
    }

    private static void ConnectCallback(IAsyncResult ar)
    {
        try
        {
            Socket client = (Socket)ar.AsyncState;

            client.EndConnect(ar);

            Debug.Log("Socket Connected to " + client.RemoteEndPoint.ToString());

            connectDone.Set();
        }
        catch(Exception e)
        {
            Debug.Log(e.ToString());
        }
    }

    private static void Receive(Socket client)
    {
        try
        {
            StateObject state = new StateObject();
            state.workSocket = client;

            client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReceiveCallback), state);
        }
        catch(Exception e)
        {
            Debug.Log(e.ToString());
        }
    }

    private static void ReceiveCallback(IAsyncResult ar)
    {
        try
        {
            StateObject state = (StateObject)ar.AsyncState;
            Socket client = state.workSocket;

            int bytesRead = client.EndReceive(ar);

            if(bytesRead>0)
            {
                state.sb.Append(Encoding.ASCII.GetString(state.buffer, 0, bytesRead));

                client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReceiveCallback), state);
            }
            else
            {
                if(state.sb.Length > 1)
                {
                    response = state.sb.ToString();
                }

                receiveDone.Set();
            }
        }
        catch(Exception e)
        {
            Debug.Log(e.ToString());
        }
    }

    private static void Send(Socket client, string data)
    {
        byte[] byteData = Encoding.ASCII.GetBytes(data);

        client.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallback), client);
    }

    private static void SendCallback(IAsyncResult ar)
    {
        try
        {
            Socket client = (Socket)ar.AsyncState;

            int bytesSent = client.EndSend(ar);
            Debug.Log("Sent bytes to server" + bytesSent);

            sendDone.Set();
        }
        catch(Exception e)
        {
            Debug.Log(e.ToString());
        }
    }
}

#endregion

public class NetworkClient : MonoBehaviour
{
    private Socket socket;
    public string address = "192.168.0.6";
    public const int port = 9000;

    private int sendSize;
    private int recvSize;

    private byte[] sendBytes;
    private byte[] recvBytes = new byte[64];
    private string recvString;

    string[] datas;

    public ConcurrentQueue<byte[]> sendList = new ConcurrentQueue<byte[]>();

    private void Init()
    {
        socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.SendTimeout, 10000);
        socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReceiveTimeout, 10000);
    }

    private void Connect()
    {
        try
        {
            IPAddress ipAddress = IPAddress.Parse(address);
            IPEndPoint ipEndPoint = new IPEndPoint(ipAddress, port);
            socket.Connect(ipEndPoint);

            GameObject.Find("OnOff").GetComponent<Image>().color = Color.blue;
        }
        catch (Exception e)
        {
            Debug.Log("Socket connect error " + e.ToString());
            return;
        }
    }

    private bool Send()
    {
        try
        {
            if (!sendList.IsEmpty)
            {
                sendList.TryDequeue(out sendBytes);
                sendSize = Encoding.ASCII.GetString(sendBytes).Length;
                if (socket.Send(sendBytes, sendSize, 0, 0) > 0)
                {
                    Debug.Log("Send " + sendBytes.ToString());
                    return true;
                }
                else
                {
                    Debug.Log("Send failed");
                    return false;
                }
            }
            else
            {
                sendSize = Encoding.ASCII.GetByteCount(DateTime.Now.ToString());
                sendBytes = Encoding.ASCII.GetBytes(DateTime.Now.ToString());
                if (socket.Send(sendBytes, sendSize, 0) > 0)
                {
                    Debug.Log("Send success");
                    return true;
                }
                else
                {
                    Debug.Log("Send Failed");
                    return false;
                }
            }
        }
        catch (SocketException e)
        {
            Debug.Log("Socket send error " + e.ToString());
            return false;
        }
         catch (Exception e)
        {
            Debug.Log("error " + e.ToString());
            return false;
        }

    }

    private bool Recv()
    {
        try
        {
            if(socket.Receive(recvBytes) > 0)
            {
                Debug.Log(recvBytes.Length);
                ParseBytes(Encoding.Default.GetString(recvBytes));
                return true;
            }
            else
            {
                Debug.Log("Send Failed");
                return false;
            }
            
        }
        catch(SocketException e)
        {
            Debug.Log("Socket receive error " + e.ToString());
            return false;
        }
        catch(Exception e)
        {
            Debug.Log("Error " + e.ToString());
            return false;
        }
    }

    private void ParseBytes(string recvData)
    {
        datas = recvData.Split('`');
        foreach(string data in datas)
        {
            Debug.Log(data + ' ');
        }
    }

    public GameObject chatContentPref;


    public void PushSendData(byte[] data)
    {
        sendList.Enqueue(data);
    }

    public void OnSendBtnClicked()
    {
        InputField input = GameObject.Find("InputField").GetComponent<InputField>();
        if (input.text.Length > 0)
        {
            PushSendData(Encoding.UTF8.GetBytes(input.text.ToString()));

            GameObject pref = Instantiate(chatContentPref);
            chatContentPref.GetComponentInChildren<Text>().text = input.text;
            
            pref.transform.parent = GameObject.Find("Content").transform;
            input.text = "";
        }
    }

    private void Awake()
    {
        Init();
        Connect();
        InvokeRepeating(nameof(Send), 0.0f, 1f);
        InvokeRepeating(nameof(Recv), 0.0f, 1f);
    }

    private void Update()
    {
        if(Input.GetKeyDown(KeyCode.Space))
        {
            OnSendBtnClicked();
        }
    }
}
