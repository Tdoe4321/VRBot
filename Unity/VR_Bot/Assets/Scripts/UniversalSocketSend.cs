using UnityEngine;
using System.Text;
using System.Threading;
using System.Collections;
using System.Timers;
using NetMQ; // for NetMQConfig
using NetMQ.Sockets;

public class UniversalSocketSend : MonoBehaviour {

    public GameObject leftJoy;
    public GameObject rightJoy;

    string final;

    private bool sendingData = true;

    Thread client_thread_;
    private Object thisLock_ = new Object();
    bool stop_thread_ = false;

    public string msg;

    PublisherSocket pubSocket;

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

        pubSocket = new PublisherSocket();
        pubSocket.Options.ReceiveHighWatermark = 0;
        pubSocket.Connect("tcp://192.168.1.122:55556");

        bool is_connected = true;

        while (is_connected && stop_thread_ == false) {
            pubSocket.TrySendFrame(timeout, final);

        }

        pubSocket.Close();
        Debug.Log("ContextTerminate.");
        NetMQConfig.ContextTerminate();
        NetMQConfig.Cleanup();
    }

    void Update() {
        if (sendingData == true) {
            string leftJoyString = leftJoy.GetComponent<CalcDistJoy>().scaledDist.ToString();
            string rightJoyString = rightJoy.GetComponent<CalcDistJoy>().scaledDist.ToString();
            string hornString = "1";

            //final = leftJoyString + ":" + rightJoyString + ">";
            final = leftJoyString + ":" + rightJoyString + ":" + hornString + ">";

            Debug.Log(final);
        }
    }

    void OnApplicationQuit() {

        pubSocket.TrySendFrame(new System.TimeSpan(0, 0, 1), "0:0:0>");

        lock (thisLock_) stop_thread_ = true;
        client_thread_.Join();
        Debug.Log("Quit the thread.");
    }

}
