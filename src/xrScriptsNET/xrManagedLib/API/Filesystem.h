#pragma once
#include "Defines.h"

using namespace System;

namespace XRay
{
	public ref class File sealed : public IO::Stream
	{
	public:
		virtual int Read(array<unsigned char>^ buffer, int offset, int count) override;
		virtual void Write(array<unsigned char>^ buffer, int offset, int count) override;

		virtual property bool CanRead
		{
			bool get() override
			{
				return pReader != nullptr;
			}
		}

		virtual property bool CanWrite
		{
			bool get() override
			{
				return pWritter != nullptr;
			}
		}

		virtual property bool CanSeek
		{
			bool get() override
			{
				return pReader != nullptr;
			}
		}

		virtual property Int64 Length
		{
			Int64 get() override
			{
				if (pReader != nullptr)
				{
					return pReader->length();
				}
				if (pWritter != nullptr)
				{
					return pWritter->tell();
				}
				return 0;
			}
		}

		virtual property Int64 Position
		{
			Int64 get() override
			{
				if (pReader != nullptr)
				{
					return pReader->tell();
				}
				if (pWritter != nullptr)
				{
					return pWritter->tell();
				}
				return 0;
			}

			void set(Int64 value) override
			{
				if (pReader != nullptr)
				{
					pReader->seek((Int32)value);
				}

				// Code for written for future cases
				if (pWritter != nullptr)
				{
					pWritter->seek((Int32)value);
				}
			}
		}

		property IntPtr NativeReader
		{
			IntPtr get()
			{
				return (IntPtr)pReader;
			}
		}

		property IntPtr NativeWritter
		{
			IntPtr get()
			{
				return (IntPtr)pWritter;
			}
		}

		File(IntPtr nativeReader);
		virtual ~File();

		virtual void Flush() override;
		virtual void SetLength(Int64 value) override;

		virtual Int64 Seek(Int64 offset, System::IO::SeekOrigin origin) override;

		static File^ OpenRead(String^ filename);
		static File^ OpenWrite(String^ filename);

		static File^ OpenExternalRead(String^ filename);
		static File^ OpenExternalWrite(String^ filename);

	protected:
		File();

		IReader* pReader = nullptr;
		IWriter* pWritter = nullptr;
	};


	public ref class Filesystem abstract sealed
	{
	public:
		static bool IsFileExist(String^ filename);
		static bool IsExternalFileExist(String^ filename);
		static String^ GetPathToResource(ResourceType Type, String^ filename);

		static void ValidateStringAndGetAsciiCopy(String^ filename, string1024& OutFileNameBuff);
	};
}