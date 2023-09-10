using System;
using System.Runtime.CompilerServices;

namespace Iota
{
	public class NativeCall
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern bool IsInitialized();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern bool IsRunning();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void SetFrameLimit(uint frameLimit);
	}

	public class Application
	{
		public static bool IsInitialized() { return NativeCall.IsInitialized(); }
		public static bool IsRunning() { return NativeCall.IsRunning(); }
		public static void SetFPS(uint fps)
		{
			NativeCall.SetFrameLimit(fps);
		}
	}
}