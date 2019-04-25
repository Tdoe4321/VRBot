using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;
using System;
using Valve.VR.InteractionSystem;

public class HornButton : MonoBehaviour {

    public int hornPressed = 0;

    public void OnButtonDown(Hand fromHand) {
        ColorSelf(Color.cyan);
        fromHand.TriggerHapticPulse(1000);
        hornPressed = 1;
    }

    public void OnButtonUp(Hand fromHand) {
        ColorSelf(Color.white);
        hornPressed = 0;
    }

    private void ColorSelf(Color newColor) {
        Renderer[] renderers = this.GetComponentsInChildren<Renderer>();
        for (int rendererIndex = 0; rendererIndex < renderers.Length; rendererIndex++) {
            renderers[rendererIndex].material.color = newColor;
        }
    }
}