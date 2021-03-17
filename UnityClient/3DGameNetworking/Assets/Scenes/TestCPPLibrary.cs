using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class TestCPPLibrary : MonoBehaviour
{
    [DllImport("NetworkClient_Ver02")]
    public static extern bool Init(int interval, int port);

    [DllImport("NetworkClient_Ver02")]
    public static extern bool Connect();

    [DllImport("NetworkClient_Ver02")]
    public static extern string PopDataInDeque();

    [DllImport("NetworkClient_Ver02")]
    public static extern bool Update();

    // Start is called before the first frame update
    void Start()
    {
        if (Init(1, 9000))
        {
            InvokeRepeating(nameof(ClientUpdate), 0.0f, 0.5f);
        }
        else
        {
            Debug.Log("Init() failed");
        }
    }

    void ClientUpdate()
    {
        if(Connect())
        {
            if (Update())
            {
                Debug.Log(PopDataInDeque());
            }
            else
            {
                Debug.Log("Client Update() failed");
            }
        }
        else
        {
            Debug.Log("Client Connect() failed");
        }
    }
}
