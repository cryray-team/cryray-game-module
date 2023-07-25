#pragma once
#include "Filesystem.h"
using namespace System::Numerics;

namespace XRay
{
	/// <summary> X-Ray CInifile wrapper </summary>
	public ref class Ini sealed
	{
	internal:
		CInifile* pNativeIni;

	public:
		/// <summary> Returns file name </summary>
		property String^ FileName
		{
			virtual String^ get() 
			{
				return gcnew String(pNativeIni->fname());
			}
		};

		Ini(String^ fileName);
		Ini(String^ fileName, bool readOnly);
		Ini(String^ fileName, bool readOnly, bool load);
		Ini(String^ fileName, bool readOnly, bool load, bool saveAtEnd);

		virtual ~Ini();

		/// <summary> Check if there is such a section </summary>
		bool IsSectionExist(String^ section);

		/// <summary> Check if there is such a line </summary>
		bool IsLineExist(String^ section, String^ line);

		/// <summary> Returns lines count on section </summary>
		UInt32 GetLineCount(String^ section);

		/// <summary> Returns boolean value </summary>
		bool ReadBool(String^ section, String^ line);

		/// <summary> Returns engine class id value </summary>
		UInt64 ReadClassId(String^ section);
		UInt64 ReadClassId(String^ section, String^ line);

		/// <summary> Returns char value </summary>
		SByte ReadByte(String^ section, String^ line);
		/// <summary> Returns unsigned char value </summary>
		Byte ReadUByte(String^ section, String^ line);

		/// <summary> Returns short value </summary>
		Int16 ReadShort(String^ section, String^ line);
		/// <summary> Returns unsigned short value </summary>
		UInt16 ReadUShort(String^ section, String^ line);

		/// <summary> Returns int value </summary>
		Int32 ReadInt(String^ section, String^ line);
		/// <summary> Returns unsigned int value </summary>
		UInt32 ReadUInt(String^ section, String^ line);

		/// <summary> Returns long value </summary>
		UInt64 ReadULong(String^ section, String^ line);

		/// <summary> Returns float value </summary>
		float ReadFloat(String^ section, String^ line);

		/// <summary> Returns color value </summary>
		Int32 ReadColor(String^ section, String^ line);

		/// <summary> Returns Vector2 value </summary>
		Vector2^ ReadVector2(String^ section, String^ line);

		/// <summary> Returns Vector3 value </summary>
		Vector3^ ReadVector3(String^ section, String^ line);

		/// <summary> Returns Vector4 value </summary>
		Vector4^ ReadVector4(String^ section, String^ line);

		/// <summary> Returns string value </summary>
		String^ ReadString(String^ section, String^ line);

		void SaveAs(String^ fileName);

		virtual String^ ToString() override;
	};
}