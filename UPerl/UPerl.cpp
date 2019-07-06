///////////////////////////////////////////////////////////
// UPerl.cpp
// Source for UPerl Class - A dotNet wrapper around Perl
// Author: Chad G. Singer
// Copyright (c) 2016 USAN Solutions
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uperl.h"

#define g_malloc malloc
#define DLLEXPORT extern "C" __declspec(dllexport)

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection;

EXTERN_C void boot_DynaLoader(pTHX_ CV* cv);

EXTERN_C void xs_init(pTHX) 
{
	char *file = __FILE__;
	dXSUB_SYS;
	newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, file);
}

namespace UPerl 
{
	UPerlInterpreter::UPerlInterpreter(void) 
	{
		// Create static object..
		LocalInterpreter = uPerlInit(0,0);
	}

	UPerlInterpreter::~UPerlInterpreter(void) 
	{
		if (LocalInterpreter)
		{
			// "Destroy." she said.
			uPerlDestruct(LocalInterpreter);
			LocalInterpreter = NULL;
		}
	}

	void UPerlInterpreter::LoadModule(System::String ^ ModuleName, System::String ^ ModuleArgs)
	{
		// Convert variables..
		SV *modNameSv = Perl_newSVpv(LocalInterpreter, (char*)(void*)Marshal::StringToHGlobalAnsi(ModuleName), ModuleName->Length);
		SV *modArgsSv = Perl_newSVpv(LocalInterpreter, (char*)(void*)Marshal::StringToHGlobalAnsi(ModuleArgs), ModuleArgs->Length);

		// Load the module..
		load_module(NULL, modNameSv, NULL, modArgsSv, NULL);
		
		// Return
		return;
	}
	
	System::String ^ UPerlInterpreter::RunScript(System::String ^ ScriptText)
	{
		// The Return Value Var
		System::String ^ returnVal = gcnew System::String("");

		// Convert the input to a char*
		char* aInput = (char*)(void*)Marshal::StringToHGlobalAnsi(ScriptText);

		// If the damn thing isn't null.
		if (LocalInterpreter)
		{
			// RetVal
			void *result = NULL;			
			
			// Call Perl
			result = Perl_eval_pv(LocalInterpreter, (const char*)aInput, 0);

			// BARFFF
			if (!result)
			{			
				System::Console::WriteLine(String::Format("Error: Script [{0}] returned NULL.", ScriptText));
				return returnVal;
			}
			
			// Get the response
			char* tResult = uPerlSVReftype(LocalInterpreter,(SV *)result);

			// Convert to System::String
			System::String ^ type = gcnew System::String(tResult);
						
			if (type == "SCALAR")
			{			
				char* sResult = uPerlSvPV(LocalInterpreter, (SV *)result);			
				returnVal = String::Format("{0}", gcnew System::String(sResult));
				return returnVal;
			} 			
		}

		return  returnVal;
	}

	PerlInterpreter * UPerlInterpreter::uPerlInit(int t, int w)
	{
		// Init.
		PERL_SYS_INIT3(&__argc, &__argv, &_environ);	

		// Call the API to get an 'environment'
		PerlInterpreter *my_perl = perl_alloc();

		// Finish up environment
		perl_construct(my_perl);
		
		// Why is this here?
		PTHREAD_ATFORK(Perl_atfork_lock, Perl_atfork_unlock, Perl_atfork_unlock);

		// Commandline arguments for the perl interpreter (just like on the cli)
		char *embedding[] = { "", "-e", "0" };
		
		// Parse that junk
		perl_parse(my_perl, xs_init, 3, embedding, _environ);

		// Set up stuff
		my_perl->Iperl_destruct_level = 1;
		my_perl->Iexit_flags |= PERL_EXIT_DESTRUCT_END;

		// Automatically 'Use' some junk.



		// Back to reality
		return my_perl;
	}
	
	// Called by UPerlInterpreter destructor
	void UPerlInterpreter::uPerlDestruct(PerlInterpreter * my_perl) 
	{
		PERL_SET_CONTEXT(my_perl);
		my_perl->Iperl_destruct_level = 0;
		my_perl->Iexit_flags |= PERL_EXIT_DESTRUCT_END;
	}

	// Takes in an 'SV' (Scalar Value) and shats out a char* style string 
	char *UPerlInterpreter::uPerlSvPV(PerlInterpreter *my_perl, SV* sv) 
	{
		STRLEN len;
		char *str = SvPV(sv, len);
		char *ret = (char*)g_malloc(len + 1);
		memcpy(ret, str, len);
		ret[len] = 0;
		return ret;
	}
	
	char *UPerlInterpreter::uPerlSVReftype(PerlInterpreter *my_perl, SV *sv) 
	{
		char *pret = (char *)Perl_sv_reftype(my_perl, sv, 0);
		char *ret = (char*)g_malloc(strlen(pret) + 1);
		strcpy(ret, pret);
		return ret;
	}
}