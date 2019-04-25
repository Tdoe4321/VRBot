using UnityEngine;
using System.Text;
using System.Threading;
using System.Collections;
using System.Timers;
using NetMQ; // for NetMQConfig
using NetMQ.Sockets;
using OpenCvSharp;

public class ZMQSUB : MonoBehaviour {

    Thread client_thread_;
    private Object thisLock_ = new Object();
    bool stop_thread_ = false;

    public string msg;

    //Mat image;

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

        var subSocket = new SubscriberSocket();
        subSocket.Options.ReceiveHighWatermark = 0;
        subSocket.Connect("tcp://192.168.1.122:55555");
        subSocket.Subscribe("");

        bool is_connected = true;

        while (is_connected && stop_thread_ == false) {
            is_connected = subSocket.TryReceiveFrameString(timeout, out msg);

            //Debug.Log(msg);
            //byte[] decodedBytes = System.Convert.FromBase64String(msg);
            //Debug.Log(decodedBytes);
            //string decodedText = Encoding.UTF8.GetString(decodedBytes);
            //Debug.Log(decodedText);
            //Mat buffer = new Mat(decodedText, ImreadModes.Unchanged);
            //image = Cv2.ImDecode(buffer, ImreadModes.Unchanged);
            //using (var window = new Window("window", image: src, flags: WindowMode.AutoSize)) {
            //Cv2.WaitKey();
            //}
            //Debug.Log(src);
        }

        subSocket.Close();
        Debug.Log("ContextTerminate.");
        NetMQConfig.ContextTerminate();
        NetMQConfig.Cleanup();
    }


    void Update() {

    }

    void OnApplicationQuit() {
        lock (thisLock_) stop_thread_ = true;
        client_thread_.Join();
        Debug.Log("Quit the thread.");
    }

}