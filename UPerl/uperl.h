///////////////////////////////////////////////////////////
// UPerl.h
// Header for UPerl Class - A dotNet wrapper around Perl
// Author: Chad G. Singer
// Copyright (c) 2016 USAN Solutions
///////////////////////////////////////////////////////////

#include "Stdafx.h"

#pragma once

using namespace System;

namespace UPerl 
{

	public ref class UPerlInterpreter
	{
		public:
			
			// The Perl Interpreter
			PerlInterpreter* LocalInterpreter = NULL;

			// Support functions and Perl interface
			PerlInterpreter *uPerlInit(int t, int w);

			void uPerlDestruct(PerlInterpreter * my_perl);

			char *uPerlSvPV(PerlInterpreter *my_perl, SV* sv);

			char *uPerlSVReftype(PerlInterpreter *my_perl, SV *sv);
		
			void LoadModule(System::String ^ ModuleName, System::String ^ ModuleArgs);

			System::String ^ RunScript(System::String ^ ScriptText);	

			UPerlInterpreter();

			~UPerlInterpreter();
	};
}
