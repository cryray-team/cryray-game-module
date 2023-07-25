#include "stdafx.h"
#include "FileSystem.h"
#include "Log.h"
#define XR_MODULE "[xrScriptsNET]"
#include "../../xrCore/FS_internal.h"

using namespace XRay;
using namespace System::Runtime::InteropServices;

int File::Read(array<unsigned char>^ buffer, int offset, int count)
{
	if (pReader == nullptr)
	{
		throw gcnew System::InvalidOperationException(gcnew String("You can't read null file"));
	}

	int Elapsed = pReader->elapsed();
	// We should return 0  if stream ended, as said in specification
	if (Elapsed == 0) return 0;

	if (offset + count > buffer->Length)
	{
		throw gcnew System::InvalidOperationException(gcnew String("Buffer doesn't have enough space to hold requested amount data"));
	}

	int DataToBeReaded = std::min(Elapsed, count);
	Marshal::Copy(IntPtr(pReader->pointer()), buffer, offset, DataToBeReaded);
	return DataToBeReaded;
}

void File::Write(array<unsigned char>^ buffer, int offset, int count)
{
	if (pWritter == nullptr)
	{
		throw gcnew System::InvalidOperationException(gcnew String("You can't write null file"));
	}

	if (offset + count > buffer->Length)
	{
		throw gcnew System::InvalidOperationException(gcnew String("You provide a buffer, with less data that required"));
	}

	pin_ptr<unsigned char> bufferPtr = &buffer[offset];
	pWritter->w(bufferPtr, count);
}

File^ File::OpenRead(String^ filename)
{
	string1024 FileNameBuff;
	Filesystem::ValidateStringAndGetAsciiCopy(filename, FileNameBuff);

	File^ file = gcnew File();
	IReader* pFile = FS.r_open(FileNameBuff);
	if (pFile == nullptr)
	{
		return nullptr;
	}
	file->pReader = pFile;

	return file;
}

File^ File::OpenWrite(String^ filename)
{
	string1024 FileNameBuff;
	Filesystem::ValidateStringAndGetAsciiCopy(filename, FileNameBuff);

	File^ file = gcnew File();
	IWriter* pFile = FS.w_open(FileNameBuff);
	if (pFile == nullptr)
	{
		return nullptr;
	}
	file->pWritter = pFile;
	return file;
}


void File::Flush()
{
	if (pWritter != nullptr)
	{
		pWritter->flush();
	}
}

System::Int64 File::Seek(Int64 offset, System::IO::SeekOrigin origin)
{
	if (pReader != nullptr)
	{
		switch (origin)
		{
		case System::IO::SeekOrigin::Begin:
			pReader->seek((Int32)offset);
			break;
		case System::IO::SeekOrigin::Current:
			pReader->advance((Int32)offset);
			break;
		case System::IO::SeekOrigin::End:
			pReader->seek(pReader->length() - (Int32)offset);
			break;
		default:
			break;
		}
	}

	if (pWritter != nullptr)
	{
		switch (origin)
		{
		case System::IO::SeekOrigin::Begin:
			pWritter->seek(offset);
			break;
		case System::IO::SeekOrigin::Current:
			throw gcnew System::InvalidOperationException(gcnew String("You can't advance writter position"));
			break;
		case System::IO::SeekOrigin::End:
			pWritter->seek(pWritter->tell() - offset);
			break;
		default:
			break;
		}
	}

	return 0;
}

void File::SetLength(Int64 value)
{
	throw gcnew System::NotImplementedException(gcnew String("SetLength in XRay::File is not implemented"));
}

File::File(IntPtr nativePtr)
{
	pReader = (IReader*)nativePtr.ToPointer();
	pWritter = (IWriter*)pReader;
}

File::~File()
{
	pReader->close();
	delete pWritter;
}

File^ File::OpenExternalRead(String^ filename)
{
	string1024 FilenameBuff;
	Filesystem::ValidateStringAndGetAsciiCopy(filename, FilenameBuff);

	File^ file = gcnew File();
	file->pReader = xr_new<CFileReader>(FilenameBuff);

	return file;
}

File^ XRay::File::OpenExternalWrite(String^ filename)
{
	throw gcnew System::NotImplementedException(gcnew String("OpenExternalWrite in XRay::File is not implemented"));
}

XRay::File::File()
{}

void Filesystem::ValidateStringAndGetAsciiCopy(String^ filename, string1024& OutFileNameBuff)
{
	if (filename == nullptr)
	{
		throw gcnew System::ArgumentException(gcnew String("Filename can't be null"));
	}

	pin_ptr<const wchar_t> pinFilename = PtrToStringChars(filename);
	if (filename->Length > 1023)
	{
		Msg("! [SPECTRE] can't open file '%S' - path to long (more than 1023 characters)", pinFilename);
		throw gcnew System::ArgumentException(gcnew String("Filename is too big"));
	}

	ConvertWidecharToAscii(pinFilename, filename->Length, OutFileNameBuff);
}

bool Filesystem::IsFileExist(String^ filename)
{
	string1024 FileNameBuff;
	ValidateStringAndGetAsciiCopy(filename, FileNameBuff);

	return FS.exist(FileNameBuff) != nullptr;
}

bool Filesystem::IsExternalFileExist(String^ filename)
{
	return System::IO::File::Exists(filename);
}

System::String^ Filesystem::GetPathToResource(ResourceType Type, String^ filename)
{
	string1024 FileNameBuff;
	ValidateStringAndGetAsciiCopy(filename, FileNameBuff);
	LPSTR ResourceName = nullptr;
	switch (Type)
	{
	case XRay::MESH:
		ResourceName = "$game_meshes$";
		break;
	case XRay::DETAIL_MESH:
		ResourceName = "$game_dm$";
		break;
	case XRay::ANIM:
		ResourceName = "$game_anims$";
		break;
	case XRay::SHADER:
		ResourceName = "$game_shaders$";
		break;
	case XRay::SOUND:
		ResourceName = "$game_sounds$";
		break;
	case XRay::TEXTURE:
		ResourceName = "$game_textures$";
		break;
	case XRay::CONFIG:
		ResourceName = "$game_config$";
		break;
	case XRay::WEATHER:
		ResourceName = "$game_weathers$";
		break;
	case XRay::LEVEL:
		ResourceName = "$game_levels$";
		break;
	case XRay::SCRIPT:
		ResourceName = "$game_scripts$";
		break;
	default:
		gcnew System::ArgumentException(gcnew String("ResourceType have invalid value"));
		break;
	}
	string_path pathToFile;
	FS.update_path(pathToFile, ResourceName, FileNameBuff);

	return gcnew String(pathToFile);
}
