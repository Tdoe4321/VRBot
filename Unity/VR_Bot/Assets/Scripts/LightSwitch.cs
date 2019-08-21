using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;
using System;
using Valve.VR.InteractionSystem;

public class LightSwitch : MonoBehaviour {

    public int lightOn = 0;

    float localAngle = -45;

    // Update is called once per frame
    void Update () {
        localAngle = gameObject.GetComponent<CircularDrive>().outAngle;

        if (localAngle < 0) {
            lightOn = 0;
        }
        else if (localAngle > 0) {
            lightOn = 1;
        }
    }
}
