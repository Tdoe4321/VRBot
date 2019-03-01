using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class CalcDistJoy : MonoBehaviour {

    public Transform start;
    public Transform end;
    private Vector3 mid;

    float dist = 0;
    float totalDist = 0;
    public int scaledDist;

    public float scale(float OldMin, float OldMax, float NewMin, float NewMax, float OldValue) {

        float OldRange = (OldMax - OldMin);
        float NewRange = (NewMax - NewMin);
        float NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin;

        return (NewValue);
    }

    // Use this for initialization
    void Awake() {
        mid = (start.transform.position + end.transform.position) * 0.5f;
        totalDist = Vector3.Distance(start.position, end.position);
    }

    void Start() {

    }

    // Update is called once per frame
    void Update() {

        dist = Vector3.Distance(mid, transform.position);

        if (Vector3.Distance(start.position, transform.position) > Vector3.Distance(end.position, transform.position)) {
            dist *= -1;
        }

        scaledDist = (int)(scale(-1 * (totalDist / 2), totalDist / 2, -255, 255, dist));

    }

    void OnApplicationQuit() {
        scaledDist = 0;
    }
}
