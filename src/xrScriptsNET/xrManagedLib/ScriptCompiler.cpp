#include "stdafx.h"
#include "ScriptCompiler.h"
#include "API/Log.h"
#define XR_MODULE "[xrScriptsNET]"

using namespace System::Reflection;
using namespace System::IO;
using namespace System::CodeDom::Compiler;
using namespace Microsoft;

xrScriptCompiler::xrScriptCompiler()
{
	PrivateInfo = new xrScriptCompiler_Private();
	CSSourceCodes = gcnew List<String^>();
	VBSourceCodes = gcnew List<String^>();
	Parameters = gcnew CompilerParameters();
}

xrScriptCompiler::~xrScriptCompiler()
{
	delete PrivateInfo;
}

CompilerResults^ xrScriptCompiler::FindCSScripts()
{
	string_path ScriptFolder;
	FS.update_path(ScriptFolder, "$game_csharp$", "");

	FS_FileSet CSScriptFiles;
	FS.file_list(CSScriptFiles, ScriptFolder, FS_ListFiles, "*.cs");

	if (CSScriptFiles.empty())
	{
		Log("SpectreC#Compiler: nothing to compile, skip");
		return nullptr;
	}

	for (const FS_File& file : CSScriptFiles)
	{
		string_path scriptPath;
		FS.update_path(scriptPath, "$game_csharp$", file.name.c_str());

		CSSourceCodes->Add(gcnew String(scriptPath));
	}

	Msg("SpectreC#Compiler: compiling...");

	CSharp::CSharpCodeProvider^ provider = gcnew CSharp::CSharpCodeProvider();
	return provider->CompileAssemblyFromFile(Parameters, CSSourceCodes->ToArray());
}

CompilerResults^ xrScriptCompiler::FindVBScripts()
{
	string_path ScriptFolder;
	FS.update_path(ScriptFolder, "$game_csharp$", "");

	FS_FileSet VBScriptFiles;
	FS.file_list(VBScriptFiles, ScriptFolder, FS_ListFiles, "*.vb");

	if (VBScriptFiles.empty())
		return nullptr;

	for (const FS_File& file : VBScriptFiles)
	{
		string_path scriptPath;
		FS.update_path(scriptPath, "$game_csharp$", file.name.c_str());

		VBSourceCodes->Add(gcnew String(scriptPath));
	}

	// Add C# scripts module
	Parameters->OutputAssembly = GetPathToBuildAssembly("xrExternalDotScripts.dll");
	Parameters->ReferencedAssemblies->Add(GetPathToBuildAssembly("xrDotScripts.dll"));

	Msg("SpectreVBCompiler: compiling...");

	// Build VB.NET scripts
	VisualBasic::VBCodeProvider^ provider = gcnew VisualBasic::VBCodeProvider();
	return provider->CompileAssemblyFromFile(Parameters, VBSourceCodes->ToArray());
}

bool xrScriptCompiler::ErrorHadler(CompilerResults^ result)
{
	if (!result)
		return false;

	// Print errors
	if (result->Errors->HasErrors)
	{
		Msg("! CSharp compilation failed:");

		for (int errInd = 0; errInd < result->Errors->Count; errInd++)
		{
			CompilerError^ error = result->Errors[errInd];
			Msg("  * %s\n", error->ToString());
		}

		return false;
	}

	// #TODO: Implement warnings...


	Assembly^ ScriptModule = nullptr;
	try
	{
		ScriptModule = Assembly::LoadFile(Parameters->OutputAssembly);
	}
	catch (FileNotFoundException^ fileNotFound)
	{
		Msg("! SpectreC#Compiler can't load compiled file. Can't write to disk (Not enough space?)");
		XRay::Log::Error(fileNotFound->FusionLog);
		return false;
	}
	catch (FileLoadException^ FileLoadExp)
	{
		Msg("! SpectreC#Compiler can't load compiled file. Incorrect file format");
		XRay::Log::Error(FileLoadExp->FusionLog);
		return false;
	}
	catch (ArgumentNullException^)
	{
		Msg("! SpectreC#Compiler can't compile source code");
		return false;
	}
	catch (BadImageFormatException^ invalidImageExp)
	{
		Msg("! SpectreC#Compiler can't load compiled file. Incorrect file format");
		XRay::Log::Error(invalidImageExp->FusionLog);
		return false;
	}

	if (ScriptModule == nullptr)
	{
		Msg("! SpectreC#Compiler can't compile source code");
		return false;
	}

	scriptAssembly = ScriptModule;

	Msg("SpectreScriptCompiler: compilation succeed.");

	return scriptAssembly != nullptr;
}

bool xrScriptCompiler::CompileScripts()
{
	const bool spectreDebug = strstr(Core.Params, "-spectre_debug") ? true : false;

	Parameters->ReferencedAssemblies->Add(GetPathToThisAssembly());
	Parameters->ReferencedAssemblies->Add(GetPathToBuildAssembly("xrManagedEngineLib.dll"));
	Parameters->ReferencedAssemblies->Add(GetPathToBuildAssembly("xrManagedGameLib.dll"));

#if 0
	Parameters->ReferencedAssemblies->Add(GetPathToBuildAssembly("xrManagedUILib.dll"));
	Parameters->ReferencedAssemblies->Add(GetPathToBuildAssembly("xrManagedRenderLib.dll"));
#endif

	Parameters->GenerateInMemory = false;
	Parameters->GenerateExecutable = false;

	Parameters->IncludeDebugInformation = spectreDebug;
	Parameters->OutputAssembly = GetPathToBuildAssembly("xrDotScripts.dll");
	Parameters->CompilerOptions = "-platform:x64 ";

#ifdef DEBUG
	Parameters->CompilerOptions += "-define:OXY_DEBUG ";
#else
	Parameters->CompilerOptions += "-define:OXY_RELEASE ";
#endif

	if (spectreDebug)
	{
		Parameters->CompilerOptions += "-define:SPECTRE_DEBUG";
	}
	else
	{
		Parameters->CompilerOptions += "-define:SPECTRE_RELEASE";
	}

	if (ErrorHadler(FindCSScripts()))
	{
		ErrorHadler(FindVBScripts());
		return true;
	}
	
	return false;
}

System::Reflection::Assembly^ xrScriptCompiler::GetAssembly()
{
	return scriptAssembly;
}

System::String^ xrScriptCompiler::GetPathToThisAssembly()
{
	String^ codeBase = Assembly::GetExecutingAssembly()->CodeBase;
	UriBuilder^ uri = gcnew UriBuilder(codeBase);
	String^ path = Uri::UnescapeDataString(uri->Path);
	return path;
}

System::String^ xrScriptCompiler::GetPathToBuildAssembly(String^ assemblyName)
{
	String^ BinFileName = Assembly::GetCallingAssembly()->Location;
	String^ BinDirectory = System::IO::Path::GetDirectoryName(BinFileName);
	String^ LoadFilename = BinDirectory + "\\" + assemblyName;
	return LoadFilename;
}
