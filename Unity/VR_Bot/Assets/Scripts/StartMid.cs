using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StartMid : MonoBehaviour {

    public Transform obj1;
    public Transform obj2;

	// Use this for initialization
	void Start () {
        transform.position = (obj1.transform.position + obj2.transform.position) / 2;
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
