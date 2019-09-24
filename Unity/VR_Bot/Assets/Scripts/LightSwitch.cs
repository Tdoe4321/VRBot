using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;
using System;
using Valve.VR.InteractionSystem;

public class LightSwitch : MonoBehaviour {

    public int lightOn = 0;

    float localAngle = -45;

    Renderer lightRender;

    void Start() {
       lightRender = gameObject.GetComponent<Renderer>();
    }

    // Update is called once per frame
    void Update () {
        localAngle = gameObject.GetComponent<CircularDrive>().outAngle;

        if (localAngle < 0) {
            lightOn = 0;
            lightRender.material.SetColor("_Color", Color.white);
        }
        else if (localAngle > 0) {
            lightOn = 1;
            lightRender.material.SetColor("_Color", Color.blue);
        }
    }
}
