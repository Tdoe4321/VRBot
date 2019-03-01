using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Valve.VR;


public class SetStartPos : MonoBehaviour {

    public Transform resetPosition;

    // Use this for initialization
    void Start() {
        GameObject PlayerCamera = GameObject.Find("VRCamera");  //get the VRcamera object

        this.transform.rotation = Quaternion.Euler(0, resetPosition.transform.rotation.eulerAngles.y - PlayerCamera.transform.rotation.eulerAngles.y, 0);

        Vector3 GlobalCameraPosition = PlayerCamera.transform.position;  //get the global position of VRcamera
        Vector3 GlobalPlayerPosition = this.transform.position;
        Vector3 GlobalOffsetCameraPlayer = new Vector3(GlobalCameraPosition.x - GlobalPlayerPosition.x, 0, GlobalCameraPosition.z - GlobalPlayerPosition.z);
        Vector3 newRigPosition = new Vector3(resetPosition.transform.position.x - GlobalOffsetCameraPlayer.x, resetPosition.transform.position.y, resetPosition.transform.position.z - GlobalOffsetCameraPlayer.z);
        this.transform.position = newRigPosition;

    }


    // Update is called once per frame
    void Update() {
        if (SteamVR_Actions._default.ResetSeat.GetStateDown(SteamVR_Input_Sources.Any)) {

            GameObject PlayerCamera = GameObject.Find("VRCamera");  //get the VRcamera object

            this.transform.rotation = Quaternion.Euler(0, 0, 0);
            this.transform.rotation = Quaternion.Euler(0, resetPosition.transform.rotation.eulerAngles.y - PlayerCamera.transform.rotation.eulerAngles.y, 0);

            Vector3 GlobalCameraPosition = PlayerCamera.transform.position;  //get the global position of VRcamera
            Vector3 GlobalPlayerPosition = this.transform.position;
            Vector3 GlobalOffsetCameraPlayer = new Vector3(GlobalCameraPosition.x - GlobalPlayerPosition.x, 0, GlobalCameraPosition.z - GlobalPlayerPosition.z);
            Vector3 newRigPosition = new Vector3(resetPosition.transform.position.x - GlobalOffsetCameraPlayer.x, resetPosition.transform.position.y, resetPosition.transform.position.z - GlobalOffsetCameraPlayer.z);
            this.transform.position = newRigPosition;
        }
    }
}
