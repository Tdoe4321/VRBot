using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using OpenCvSharp;
using ICSharpCode.SharpZipLib.Zip;



public class NetworkImageTexture : MonoBehaviour {
    Renderer rend;

    string localMsg;
    byte[] decodedBytes;

    //ICSharpCode.SharpZipLib.Zip.Compression.Inflater inflate = new ICSharpCode.SharpZipLib.Zip.Compression.Inflater();

    void Start() {
        rend = this.GetComponentInChildren<Renderer>();
    }

    // Update is called once per frame
    void Update () {
        localMsg = this.GetComponentInChildren<PairSocket>().msg;
        if (localMsg != "") {
            if (localMsg[0] == '/') {
            //if (localMsg[0] == 'i') {
            //Debug.Log(localMsg);
                //ICSharpCode.SharpZipLib.Zip.Compression.Deflater de = new ICSharpCode.SharpZipLib.Zip.Compression.Deflater();

                //Debug.Log(localMsg);
                decodedBytes = System.Convert.FromBase64String(localMsg);
                //byte[] decodedBytes = System.Convert.FromBase64String(localMsg);
                //Debug.Log(decodedBytes);
                //inflate.Inflate(decodedBytes);
                //Debug.Log(decodedBytes);
                //de.Deflate(final);
                Texture2D tex = new Texture2D(1,1);
                tex.LoadImage(decodedBytes);

                rend.material.mainTexture = tex;

            }
        }
    }
}
