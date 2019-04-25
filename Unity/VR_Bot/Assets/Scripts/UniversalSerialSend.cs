using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class UniversalSerialSend : MonoBehaviour {

    public GameObject leftJoy;
    public GameObject rightJoy;

    private bool sendingData = true;
    static public SerialPort serial = new SerialPort("COM12", 115200);


    void Start() {
        if (serial.IsOpen == false) {
            serial.Open();
        }
    }

    // Update is called once per frame
    void Update() {

        if (serial.IsOpen == false) {
            serial.Open();
        }


        if (sendingData == true) {
            string leftJoyString = leftJoy.GetComponent<CalcDistJoy>().scaledDist.ToString();
            string rightJoyString = rightJoy.GetComponent<CalcDistJoy>().scaledDist.ToString();

            string final = leftJoyString + ":" + rightJoyString + ">";

            Debug.Log(final);
            serial.Write(final);
        }

    }

    void OnApplicationQuit() {
        if (serial.IsOpen == true) {
            serial.Write("0:0>");
            serial.Close();
        }
        Debug.Log("Application ending after " + Time.time + " seconds");
    }
}
