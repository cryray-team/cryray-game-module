using System;
using System.Collections.Generic;

namespace xrSpectre
{
    public class LoadTest : XRay.ModInstance
    {
        public override void OnLoad()
        {
			XRay.ClassRegistrator.Register(typeof(GamePersistence), (UInt64)XRay.EngineClassIDs.GamePersistence);
			//XRay.ClassRegistrator.Register(typeof(RenderTarget), (UInt64)XRay.EngineClassIDs.RenderTarget);
            XRay.Log.Info("! SpectreC#: Inited!");
	        //XRay.Log.Info("! SpectreC#: Engine version atring: " + XRay.System.GetVersionString);
        }

        public override void OnShutdown()
        {
            XRay.Log.Info("! SpectreC#: Shutting down");
        }
    }
}