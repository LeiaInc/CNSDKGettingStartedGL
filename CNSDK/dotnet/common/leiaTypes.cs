using System;
using System.Runtime.InteropServices;

namespace Leia
{
	public enum LogLevel
    {
        Default = 0,
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical,
        Off,
    }
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct Vector2d
    {
        public double x, y;
    }
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct Vector3
    {
        public float x, y, z;
    }
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct Timestamp
    {
        public enum Space
        {
            Unknown = -1,
            System = 0,
        };
        Space space;

        public double ms;
    }
    public enum FaceDetectorBackend
    {
        Unknown = 0,
        CPU = 1 << 0,
        GPU = 1 << 1,

        Count = 2,
    }
    public enum FaceDetectorInputType
    {
        Unknown = 0,
        CPU = 1 << 0,
        GPU = 1 << 1,

        Count = 2,
    }
}
