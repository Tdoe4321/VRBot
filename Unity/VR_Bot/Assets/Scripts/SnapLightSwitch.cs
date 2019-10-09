using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Valve.VR.InteractionSystem {

    //[RequireComponent(typeof(Interactable))]
    //[RequireComponent(typeof(CircularDrive))]
    public class SnapLightSwitch : MonoBehaviour {

        CircularDrive circular;
        private Interactable interactable;
        bool lastAttached = false;
        bool Attached = false;

        private void Awake() {
            circular = this.GetComponent<CircularDrive>();
            interactable = GetComponent<Interactable>();
        }

        void Update() {
            //Debug.Log(Attached);

            Attached = interactable.attachedToHand;

            if (lastAttached && !Attached) {
                if (gameObject.GetComponent<LightSwitch>().lightOn == 1) {
                    gameObject.GetComponent<CircularDrive>().outAngle = 45;
                    gameObject.transform.eulerAngles = new Vector3(
                        gameObject.transform.eulerAngles.x,
                        45,
                        gameObject.transform.eulerAngles.z
                    );
                    circular.linearMapping.value = 1.0f;
                }
                else if (gameObject.GetComponent<LightSwitch>().lightOn == 0) {
                    gameObject.GetComponent<CircularDrive>().outAngle = -45;
                    gameObject.transform.eulerAngles = new Vector3(
                        gameObject.transform.eulerAngles.x,
                        -45,
                        gameObject.transform.eulerAngles.z
                    );
                    circular.linearMapping.value = 0.0f;
                }
            }

            lastAttached = Attached;
        }
        /*
        private void OnDetachedFromHand(Hand hand) {

            Debug.Log("POOOOOP");
            /*
            if (gameObject.GetComponent<LightSwitch>().lightOn == 1) {
                gameObject.GetComponent<CircularDrive>().outAngle = 45;
                gameObject.transform.eulerAngles = new Vector3(
                    gameObject.transform.eulerAngles.x,
                    45,
                    gameObject.transform.eulerAngles.z
                );
                circular.linearMapping.value = 1.0f;
            }
            else if (gameObject.GetComponent<LightSwitch>().lightOn == 0) {
                gameObject.GetComponent<CircularDrive>().outAngle = -45;
                gameObject.transform.eulerAngles = new Vector3(
                    gameObject.transform.eulerAngles.x,
                    -45,
                    gameObject.transform.eulerAngles.z
                );
                circular.linearMapping.value = 0.0f;
            }
            

        }
        */
    }
}
