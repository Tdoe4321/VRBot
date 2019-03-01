using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Valve.VR.InteractionSystem {

    [RequireComponent(typeof(Interactable))]
    [RequireComponent(typeof(LinearDrive))]
    public class ReturnToStartOnLetGo : MonoBehaviour {

        public Transform returnPos;
        //public GameObject objectToReset;
        LinearDrive linearDrive;
        
        private void Awake() {
            // Get the real value of the linear drive 
            //linearDrive = objectToReset.GetComponent<LinearDrive>();
            linearDrive = this.GetComponent<LinearDrive>();
        }

        private void OnDetachedFromHand(Hand hand) {
            // when we detatch from the hand, reset the linear drive value to 1/2 and return position of object to specified position
            transform.position = returnPos.transform.position;
            linearDrive.linearMapping.value = 0.5f;
        }
    }

}
