using System;
using System.Runtime.CompilerServices;

namespace Iota
{
	public class Application
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern bool IsInitialized();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern bool IsRunning();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void SetFrameLimit(uint fps);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public static extern void Print(string msg);
	}
}