using UnityEngine;
using System.Text;
using System.Threading;
using System.Collections;
using System.Timers;
using NetMQ; // for NetMQConfig
using NetMQ.Sockets;

public class PairSocket : MonoBehaviour {

    public GameObject leftJoy;
    public GameObject rightJoy;

    string final;

    private bool sendingData = true;

    Thread client_thread_;
    private Object thisLock_ = new Object();
    bool stop_thread_ = false;

    bool is_connected = true;

    NetMQ.Sockets.PairSocket pairSocket;

    public string msg;


    void Start() {
        Debug.Log("Start a request thread.");
        client_thread_ = new Thread(NetMQClient);
        client_thread_.Start();
    }

    // Client thread which does not block Update()
    void NetMQClient() {
        AsyncIO.ForceDotNet.Force();
        NetMQConfig.ManualTerminationTakeOver();
        NetMQConfig.ContextCreate(true);

        //string msg;
        var timeout = new System.TimeSpan(0, 0, 1); //1sec

        Debug.Log("Connect to the server.");

        pairSocket = new NetMQ.Sockets.PairSocket();
        pairSocket.Options.ReceiveHighWatermark = 0;
        pairSocket.Connect("tcp://192.168.1.122:55555");

        is_connected = true;

        while (is_connected && stop_thread_ == false) {
            is_connected = pairSocket.TryReceiveFrameString(timeout, out msg);
            pairSocket.TrySendFrame(timeout, final);
        }

        pairSocket.Close();
        Debug.Log("ContextTerminate.");
        NetMQConfig.ContextTerminate();
        NetMQConfig.Cleanup();
    }


    void Update() {
        if (sendingData == true) {
            string leftJoyString = leftJoy.GetComponent<CalcDistJoy>().scaledDist.ToString();
            string rightJoyString = rightJoy.GetComponent<CalcDistJoy>().scaledDist.ToString();
            string hornString = "0";

            final = leftJoyString + ":" + rightJoyString + ":" + hornString + ">";

            Debug.Log(final);
        }
        if(is_connected == false) {
            client_thread_.Join();
            client_thread_ = new Thread(NetMQClient);
            client_thread_.Start();
        }

 
    }

    void OnApplicationQuit() {

        //pairSocket.TrySendFrame(new System.TimeSpan(0, 0, 1), "0:0>");
        pairSocket.TrySendFrame(new System.TimeSpan(0, 0, 1), "0:0:0>");

        lock (thisLock_) stop_thread_ = true;
        client_thread_.Join();
        Debug.Log("Quit the thread.");
    }

}