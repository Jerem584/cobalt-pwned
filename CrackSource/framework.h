#pragma once

#define WIN32_LEAN_AND_MEAN             

#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>

// Process iteration
#include <TlHelp32.h>

// Hooking
#pragma lib(comment, "detours.lib")
#include <detours.h>

// Hooking Utils
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)


auto addr = (uintptr_t) GetModuleHandleA( 0 );

namespace Utils { // bad habit but idgaf
	DWORD FindProcessId( const std::wstring& processName )
	{
		PROCESSENTRY32 processInfo;
		processInfo.dwSize = sizeof( processInfo );

		HANDLE processesSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
		if ( processesSnapshot == INVALID_HANDLE_VALUE ) {
			return 0;
		}

		Process32First( processesSnapshot, &processInfo );
		if ( !processName.compare( processInfo.szExeFile ) )
		{
			CloseHandle( processesSnapshot );
			return processInfo.th32ProcessID;
		}

		while ( Process32Next( processesSnapshot, &processInfo ) )
		{
			if ( !processName.compare( processInfo.szExeFile ) )
			{
				CloseHandle( processesSnapshot );
				return processInfo.th32ProcessID;
			}
		}

		CloseHandle( processesSnapshot );
		return 0;
	}
}

namespace typedefs {
	// Function that will map the driver
	typedef void( __fastcall* t_KDMapper_Run )(std::string* a1);
	// Map dll and run it using pid and data
	typedef void( __fastcall* t_Cobalt_MapAndRun )(unsigned int pid, std::string* b64);
	// Decode Base64
	typedef __int64( __fastcall* t_Cobalt_Base64 )(__int64 out, std::string* in_b64);
	// Called at each state change (imgui message)
	typedef __int64( __fastcall* t_Cobalt_StateChange )(char*, unsigned int);
	// a json method in nlohmann lib, idk which one thop
	typedef bool( __fastcall* t_nlohmann_json )(char* a1);


	typedef bool( __fastcall* t_ImGuiBegin )(const char* name, bool p_open, uint32_t flags);

	auto o_KDMapper_Run = (t_KDMapper_Run) (addr + 0xA5C20);
	auto o_Cobalt_MapAndRun = (t_Cobalt_MapAndRun) (addr + 0xAB4D0);
	auto o_Cobalt_Base64 = (t_Cobalt_Base64) (addr + 0x10A630);
	auto o_Cobalt_StateChange = (t_Cobalt_StateChange) (addr + 0xA2CF0);
	auto o_nlohmann_json = (t_nlohmann_json) (addr + 0x1A6A0);
	auto o_ImGuiBegin = (t_ImGuiBegin) (addr + 0x263D0);

	// Functions that use boost asio to connect to their ws (shit)
	typedef int( __stdcall* t_connect )(std::string* lol);
	auto o_connect = (t_connect) (addr + 0x011DA20);
};

namespace hooked_fnc
{
	__int64 hk_Cobalt_StateChange( char* a1, unsigned int a2 )
	{
		printf( "[Cobalt] State changed to = %s (%llX) (%d)\n", a1, PVOID( uintptr_t( _ReturnAddress() ) - addr ), a2 );
		return typedefs::o_Cobalt_StateChange( a1, a2 );
	}
	__int64 hk_Cobalt_Base64( __int64 a1, std::string* a2 ) {
		printf( "[Crack] a1 = %llX | a2 = %llX (%d) | b = %llX\n", a1, a2, a2->size(), 0 );
		std::ofstream out( "cobalt_dump.txt" );
		out << *a2;
		out.close();
		return typedefs::o_Cobalt_Base64( a1, a2 );
	}

	void hk_kdmapper( std::string* a1 )
	{
		printf( "[Protection] Prevented their driver from loading\n" );
		return;
	}
	int hk_connect( std::string* )
	{
		printf( "[Protection] Blocked connect for security purpose\n" );
		return 0;
	}
	bool __fastcall hk_ImGuiBegin( const char* name, bool p_open, uint32_t flags )
	{
		return typedefs::o_ImGuiBegin( std::string(name).find("cobalt") != std::string::npos ? "Crack by Theo.#1337" : name, p_open, flags );
	}
};