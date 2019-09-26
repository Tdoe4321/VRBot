using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Valve.VR.InteractionSystem.Sample {

    public class EditCruiseVar : UIElement {

        public GameObject Wall;
        public int SetSpeedToWhat = 0;

        protected override void Awake() {
            base.Awake();
        }

        protected override void OnButtonClick() {
            base.OnButtonClick();

            Wall.GetComponent<CruiseVariable>().CruiseVariableValue = SetSpeedToWhat;

        }
    }
}
