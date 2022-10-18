using System;
using System.Runtime.InteropServices;

namespace LeiaHeadTracking
{
    struct RawFaces
    {
        public int numFaces;

        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = Constants.kMaxNumFaces)]
        public RawFace[] faces;
    }
    struct DetectedFaces
    {
        public int numFaces;

        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = Constants.kMaxNumFaces)]
        public DetectedFace[] faces;
    }
    public class Frame : IDisposable
    {
        public Frame(IntPtr unmanagedHandle)
        {
            _unmanagedHandle = unmanagedHandle;
        }
        public void Dispose()
        {
            if (_unmanagedHandle != IntPtr.Zero)
            {
                leiaHeadTrackingFrameRelease(_unmanagedHandle);
                _unmanagedHandle = IntPtr.Zero;
            }
        }
        public void GetTrackingResult(out Result trackingResult)
        {
            Utils.HandleNativeCall(leiaHeadTrackingFrameGetTrackingResult(_unmanagedHandle, out trackingResult));
        }

        private IntPtr _unmanagedHandle;

        [DllImport("leiaHeadTrackingService")]
        private static extern Int32 leiaHeadTrackingFrameGetTrackingResult(IntPtr frame, out Result result);
        [DllImport("leiaHeadTrackingService")]
        private static extern Int32 leiaHeadTrackingFrameGetRawFaces(IntPtr frame, out RawFaces raw);
        [DllImport("leiaHeadTrackingService")]
        private static extern Int32 leiaHeadTrackingFrameGetDetectedFaces(IntPtr frame, out DetectedFaces detected);
        [DllImport("leiaHeadTrackingService")]
        private static extern Int32 leiaHeadTrackingFrameGetProfiling(IntPtr frame, out FrameProfiling frameProfiling);
        [DllImport("leiaHeadTrackingService")]
        private static extern void leiaHeadTrackingFrameRelease(IntPtr frame);
    }
}
