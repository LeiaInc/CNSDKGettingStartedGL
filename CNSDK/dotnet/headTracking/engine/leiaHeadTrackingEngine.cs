using System;
using System.Runtime.InteropServices;

namespace LeiaHeadTracking
{
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct KalmanCoeffs
    {
        public Vector3 a;
        public Vector3 b;
    }
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct FilterProperties
    {
        public float accelerationThreshold;
        public float survivalTimeMs;
        public float velDampingTime;
        public float minFitDist;
        public float maxFitDist;
        public float angleYMax;
        public float interocularDistanceMin;
        public float interocularDistanceMax;
        public float bufferLim;
        public Vector3 mainFaceSkew;
        public Vector3 mainFaceScale;
        public KalmanCoeffs kalman;
    }

    public class Engine : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public struct InitArgs {
            public Int32 cameraWidth;
            public Int32 cameraHeight;
            public Int32 cameraFps;
            public float cameraBinningFactor;

            public Int32 detectorMaxNumOfFaces;

            public LogLevel logLevel;

            public FilterProperties filterProperties;

            [UnmanagedFunctionPointer(CallingConvention.StdCall)]
            public delegate void on_frame_callback(IntPtr frame, IntPtr handle);
            public on_frame_callback frameListener;
            public IntPtr frameListenerUserData;

            public Int32 enablePolling;

            [UnmanagedFunctionPointer(CallingConvention.StdCall)]
            public delegate void virtual_face_hook(Timestamp timestamp, IntPtr implementMe, IntPtr handle);
            public virtual_face_hook virtualFaceHook;
            public IntPtr virtualFaceHookUserData;
        }
        public Engine(ref InitArgs initArgs)
        {
            Utils.HandleNativeCall(leiaHeadTrackingEngineInitArgs(initArgs, out _engine));
        }
        public void Dispose()
        {
            if (_engine != IntPtr.Zero)
            {
                leiaHeadTrackingEngineRelease(_engine);
                _engine = IntPtr.Zero;
            }
        }
        public void StartTracking()
        {
            Utils.HandleNativeCall(leiaHeadTrackingEngineStartTracking(_engine));
        }
        public void StopTracking()
        {
            Utils.HandleNativeCall(leiaHeadTrackingEngineStopTracking(_engine));
        }
        public void SetFilterProperties(in FilterProperties filterProperties)
        {
            Utils.HandleNativeCall(leiaHeadTrackingEngineSetFilterProperties(_engine, in filterProperties));
        }
        public void SetTrackedEyes(bool leftEye, bool rightEye)
        {
            Utils.HandleNativeCall(leiaHeadTrackingEngineSetTrackedEyes(_engine, leftEye ? 1 : 0, rightEye ? 1 : 0));
        }
        public void SetCameraTransform(Vector3 position, Vector3 rotation)
        {
            Utils.HandleNativeCall(leiaHeadTrackingEngineSetCameraTransform(_engine, position, rotation));
        }
        public void SetCameraPosition(float x, float y, float z)
        {
            Utils.HandleNativeCall(leiaHeadTrackingEngineSetCameraPosition(_engine, x, y, z));
        }
        public void SetCameraRotation(
            float r00, float r01, float r02,
            float r10, float r11, float r12,
            float r20, float r21, float r22)
        {
            Utils.HandleNativeCall(leiaHeadTrackingEngineSetCameraRotation(_engine,
                r00, r01, r02,
                r10, r11, r12,
                r20, r21, r22));
        }
        public void GetMaxNumOfDetectedFacesLimit(out Int32 maxNumOfDetectedFacesLimit)
        {
            Utils.HandleNativeCall(leiaHeadTrackingEngineGetMaxNumOfDetectedFacesLimit(_engine, out maxNumOfDetectedFacesLimit));
        }
        public void SetMaxNumOfDetectedFaces(Int32 maxNumOfDetectedFaces)
        {
            Utils.HandleNativeCall(leiaHeadTrackingEngineSetMaxNumOfDetectedFaces(_engine, maxNumOfDetectedFaces));
        }
        public Frame GetLatestFrame()
        {
            IntPtr framePtr;
            Utils.HandleNativeCall(leiaHeadTrackingEngineGetLatestFrame(_engine, out framePtr));
            if (framePtr != IntPtr.Zero) {
                return new Frame(framePtr);
            }
            return null;
        }

        private IntPtr _engine;

        [DllImport("leiaHeadTrackingEngine")]
        private static extern Int32 leiaHeadTrackingEngineInitArgs(in InitArgs init_args, out IntPtr engine);
        [DllImport("leiaHeadTrackingEngine")]
        private static extern Int32 leiaHeadTrackingEngineStartTracking(IntPtr engine);
        [DllImport("leiaHeadTrackingEngine")]
        private static extern Int32 leiaHeadTrackingEngineStopTracking(IntPtr engine);
        [DllImport("leiaHeadTrackingEngine")]
        private static extern Int32 leiaHeadTrackingEngineSetFilterProperties(IntPtr engine, in FilterProperties filterProperties);
        [DllImport("leiaHeadTrackingEngine")]
        private static extern Int32 leiaHeadTrackingEngineSetTrackedEyes(IntPtr engine, Int32 left_eye, Int32 right_eye);
        [DllImport("leiaHeadTrackingEngine")]
        private static extern Int32 leiaHeadTrackingEngineSetCameraTransform(IntPtr engine, Vector3 position, Vector3 rotation);
        [DllImport("leiaHeadTrackingEngine")]
        private static extern Int32 leiaHeadTrackingEngineSetCameraPosition(IntPtr engine, float x, float y, float z);
        [DllImport("leiaHeadTrackingEngine")]
        private static extern Int32 leiaHeadTrackingEngineSetCameraRotation(IntPtr engine,
            float r00, float r01, float r02,
            float r10, float r11, float r12,
            float r20, float r21, float r22);
        [DllImport("leiaHeadTrackingEngine")]
        private static extern Int32 leiaHeadTrackingEngineGetMaxNumOfDetectedFacesLimit(IntPtr engine, out Int32 maxNumOfDetectedFacesLimit);
        [DllImport("leiaHeadTrackingEngine")]
        private static extern Int32 leiaHeadTrackingEngineSetMaxNumOfDetectedFaces(IntPtr engine, Int32 maxNumOfDetectedFaces);
        [DllImport("leiaHeadTrackingEngine")]
        private static extern Int32 leiaHeadTrackingEngineGetLatestFrame(IntPtr engine, out IntPtr framePtr);
        [DllImport("leiaHeadTrackingEngine")]
        private static extern void leiaHeadTrackingEngineRelease(IntPtr engine);
    }
}
