using System;

namespace Iota
{
	namespace Exceptions {
		public class ExceptionClass : Exception {
			public ExceptionClass(string message) : base(message) { }
			public ExceptionClass(string message, Exception inner): base(message, inner) { }
		}
	}
}