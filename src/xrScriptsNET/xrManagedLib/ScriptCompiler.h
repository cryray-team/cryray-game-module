#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Reflection;

// Private native class
class xrScriptCompiler_Private
{
	// Hide some structs inside private declaration
	// We don't want to gargabing global space
public:

	struct SourceCodeFile
	{
		xr_string Filename;
		u32		  CRC32;
	};

	xr_vector< SourceCodeFile > SourceFiles;
};

ref class xrScriptCompiler
{
public:
	xrScriptCompiler();
	~xrScriptCompiler();

	bool CompileScripts();

	Assembly^ GetAssembly();

private:
	CodeDom::Compiler::CompilerResults^ FindCSScripts();
	CodeDom::Compiler::CompilerResults^ FindVBScripts();

	bool ErrorHadler(CodeDom::Compiler::CompilerResults^ result);

	String^ GetPathToThisAssembly();
	String^ GetPathToBuildAssembly(String^ assemblyName);

	Assembly^ scriptAssembly;
	List <String^>^ CSSourceCodes;
	List <String^>^ VBSourceCodes;

	xrScriptCompiler_Private* PrivateInfo;
	CodeDom::Compiler::CompilerParameters^ Parameters;
};