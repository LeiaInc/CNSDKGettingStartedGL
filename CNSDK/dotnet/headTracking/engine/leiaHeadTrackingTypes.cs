using System;
using System.Runtime.InteropServices;

namespace LeiaHeadTracking
{
    using FaceIdx = UInt64;

    static class Constants
    {
        public const int kRightEyeIdx = 0;
        public const int kLeftEyeIdx = 1;
        public const int kNumEyes = 2;

        public const int kMaxNumFaces = 3;
    }
    public enum Status
    {
        Success = 0,
        ErrorInvalidInstance,
        ErrorUnknown,
    }
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct MovingPoint
    {
        public Leia.Vector3 pos;
        public Leia.Vector3 vel;
    }
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct Face
    {
        public MovingPoint point;
        public Leia.Vector3 angle;
        public FaceIdx rawFaceIndex;
    }
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct RawFace
    {
        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = Constants.kNumEyes)]
        public Leia.Vector3[] eyePoints;
        public Leia.Vector3 trackingPoint;
        public FaceIdx detectedFaceIndex;
    }
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct DetectedFace
    {
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public struct Eye
        {
            public Leia.Vector2d imageCoord;
            public float depth;
        }

        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = Constants.kNumEyes)]
        public Eye[] eyes;
        Leia.Vector3 posePosition;
        Leia.Vector3 poseAngle;
    }
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct Result
    {
        public Int32 numFaces;

        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = Constants.kMaxNumFaces)]
        public Face[] faces;

        public Leia.Timestamp timestamp;

        public Int32 jumpFlag;
    }
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct FrameProfiling {
        public Int64 cameraExposureTime;
        public Int64 faceDetectorStartTime;
        public Int64 faceDetectorEndTime;
        public Int64 apiTimestamp;
    }

    public class NativeCallFailedException : Exception
    {
        public Status Status;
        public NativeCallFailedException(Status status) : base()
        {
            Status = status;
        }
        public override string Message
        {
            get => Status.ToString();
        }
    }
    static class Utils
    {
        static public void HandleNativeCall(int nativeStatus)
        {
            var status = (Status)nativeStatus;
            if (status != Status.Success)
            {
                throw new NativeCallFailedException(status);
            }
        }
    }
}
