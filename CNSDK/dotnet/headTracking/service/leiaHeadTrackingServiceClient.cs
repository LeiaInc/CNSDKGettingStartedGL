using System;
using System.Runtime.InteropServices;

namespace LeiaHeadTracking
{
    public class ServiceClient : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public struct InitArgs {
            public LogLevel logLevel;
        }
        public ServiceClient(ref InitArgs initArgs)
        {
            Utils.HandleNativeCall(leiaHeadTrackingServiceClientInitArgs(initArgs, out _serviceClient));
        }
        public void Dispose()
        {
            if (_serviceClient != IntPtr.Zero)
            {
                leiaHeadTrackingServiceClientRelease(_serviceClient);
                _serviceClient = IntPtr.Zero;
            }
        }
        public void StartTracking()
        {
            Utils.HandleNativeCall(leiaHeadTrackingServiceClientStartTracking(_serviceClient));
        }
        public void StopTracking()
        {
            Utils.HandleNativeCall(leiaHeadTrackingServiceClientStopTracking(_serviceClient));
        }
        public Frame GetLatestFrame()
        {
            IntPtr framePtr;
            Utils.HandleNativeCall(leiaHeadTrackingServiceClientGetLatestFrame(_serviceClient, out framePtr));
            if (framePtr != IntPtr.Zero) {
                return new Frame(framePtr);
            }
            return null;
        }

        private IntPtr _serviceClient;

        [DllImport("leiaHeadTrackingService")]
        private static extern Int32 leiaHeadTrackingServiceClientInitArgs(in InitArgs init_args, out IntPtr engine);
        [DllImport("leiaHeadTrackingService")]
        private static extern Int32 leiaHeadTrackingServiceClientStartTracking(IntPtr engine);
        [DllImport("leiaHeadTrackingService")]
        private static extern Int32 leiaHeadTrackingServiceClientStopTracking(IntPtr engine);
        [DllImport("leiaHeadTrackingService")]
        private static extern Int32 leiaHeadTrackingServiceClientGetLatestFrame(IntPtr engine, out IntPtr framePtr);
        [DllImport("leiaHeadTrackingService")]
        private static extern void leiaHeadTrackingServiceClientRelease(IntPtr engine);
    }
}
