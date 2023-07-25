#include "stdafx.h"
#include "Ini.h"
#include "Log.h"

using namespace XRay;

Ini::Ini(String^ fileName) : Ini(fileName, true)
{
}

Ini::Ini(String^ fileName, bool readOnly) : Ini(fileName, readOnly, true)
{
}

Ini::Ini(String^ fileName, bool readOnly, bool load) : Ini(fileName, readOnly, load, true)
{
}

Ini::Ini(String^ fileName, bool readOnly, bool load, bool saveAtEnd)
{
	string1024 Path = {};

	ConvertDotNetStringToAscii(Filesystem::GetPathToResource(CONFIG, fileName), Path);

	pNativeIni = new CInifile(Path, readOnly, load, saveAtEnd);
}

Ini::~Ini()
{
	delete pNativeIni;
}

bool Ini::IsSectionExist(String^ section)
{
	string256 Section = {};

	ConvertDotNetStringToAscii(section, Section);

	return pNativeIni->section_exist(Section);
}

bool Ini::IsLineExist(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->line_exist(Section, Line);
}

UInt32 Ini::GetLineCount(String^ section)
{
	string256 Section = {};

	ConvertDotNetStringToAscii(section, Section);

	return pNativeIni->line_count(Section);
}

bool Ini::ReadBool(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->r_bool(Section, Line);
}

UInt64 Ini::ReadClassId(String^ section)
{
	return ReadClassId(section, "class");
}

UInt64 Ini::ReadClassId(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->r_clsid(Section, Line);
}

SByte Ini::ReadByte(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->r_s8(Section, Line);
}

Byte Ini::ReadUByte(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->r_u8(Section, Line);
}

Int16 Ini::ReadShort(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->r_s16(Section, Line);
}

UInt16 Ini::ReadUShort(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->r_u16(Section, Line);
}

Int32 Ini::ReadInt(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->r_s32(Section, Line);
}

UInt32 Ini::ReadUInt(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->r_u32(Section, Line);
}

UInt64 Ini::ReadULong(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->r_u64(Section, Line);
}

float Ini::ReadFloat(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->r_float(Section, Line);
}

Int32 Ini::ReadColor(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return pNativeIni->r_color(Section, Line);
}

String^ Ini::ReadString(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	return gcnew String(pNativeIni->r_string(Section, Line));
}

Vector2^ Ini::ReadVector2(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	Fvector2 vector = pNativeIni->r_fvector2(Section, Line);
	return gcnew Vector2(vector.x, vector.y);
}

Vector3^ Ini::ReadVector3(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	Fvector3 vector = pNativeIni->r_fvector3(Section, Line);
	return gcnew Vector3(vector.x, vector.y, vector.z);
}

Vector4^ Ini::ReadVector4(String^ section, String^ line)
{
	string256 Section = {};
	string256 Line = {};

	ConvertDotNetStringToAscii(section, Section);
	ConvertDotNetStringToAscii(line, Line);

	Fvector4 vector = pNativeIni->r_fvector4(Section, Line);
	return gcnew Vector4(vector.x, vector.y, vector.z, vector.w);
}

void Ini::SaveAs(String^ fileName)
{
	string512 FileName;

	ConvertDotNetStringToAscii(fileName, FileName);

	pNativeIni->save_as(FileName);
}

String^ Ini::ToString()
{
	return "Ini: " + gcnew String(pNativeIni->get_as_string().c_str());
}