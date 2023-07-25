#pragma once

using namespace System;

namespace XRay
{
	/// <summary>
	/// Log message to file, console and debugger (if presented)
	/// </summary>
	/// <example>
	/// <code>
	/// XRay.Log.Info("This is information message");
	/// XRay.Log.Warning("This is warning!");
	/// XRay.Log.Error("Error! Error!");
	/// </code>
	/// </example>
	public ref class Log abstract sealed
	{
	public:

		/// <summary>
		/// Log message into in-game console, file and debugger (if presented).
		/// </summary>
		/// <example>
		/// <code>
		/// XRay.Log.Info("AWDA");
		/// int SomeVariable = 2;
		/// XRay.Log.Info($"SomeVariable is {SomeVariable}");
		/// </code>
		/// </example>
		/// <param name="message">A message that will be printed</param>
		static void Info(String^ message);

		/// <summary>
		/// Log message into in-game console, file and debugger (if presented)
		/// It will also have yellow color in console
		/// </summary>
		/// <example>
		/// <code>
		/// XRay.Log.Warning("AWDA");
		/// int SomeVariable = 2;
		/// XRay.Log.Warning($"SomeVariable is {SomeVariable}");
		/// </code>
		/// </example>
		/// <param name="message">A message that will be printed</param>
		static void Warning(String^ message);

		/// <summary>
		/// Log message into in-game console, file and debugger (if presented).
		/// It will also have red color in console
		/// </summary>
		/// <example>
		/// <code>
		/// XRay.Log.Error("AWDA");
		/// int SomeVariable = 2;
		/// XRay.Log.Error($"SomeVariable is {SomeVariable}");
		/// </code>
		/// </example>
		/// <param name="message">A message that will be printed</param>
		static void Error(String^ message);

		static void Break();
	};
}