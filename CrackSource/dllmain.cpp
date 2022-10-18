// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "framework.h"

static void __stdcall HookFunction( PVOID* destination, PVOID source )
{
    DetourTransactionBegin();
    DetourUpdateThread( GetCurrentThread() );
    DetourAttach( destination, source );
    ULONG res = DetourTransactionCommit();
}

void RunCrack(LPVOID)
{
    auto isKernelLoaded = reinterpret_cast<char( __fastcall* )()>(addr + 0xA5BC0)();
    printf( "[Crack] Waiting for their shit to show up\n" );
    printf( "[Crack] Is kernel loaded -> %s\n", isKernelLoaded ? "Yes" : "No" );
    Sleep( 1000 );
    // Reading usermode b64 from file
    std::ifstream t( "usermode" );
    std::stringstream buffer;
    buffer << t.rdbuf();
    auto str = buffer.str();
    std::string* p = &str;
    std::string pb = std::string();
    pb.resize( p->size() );
    
    typedefs::o_Cobalt_StateChange( (char*)"[Cracked by Theo.#1337]\nStop pasting and rating\nLook at cmd for instructions", 7);
    if ( !isKernelLoaded ) // Is kernel loaded?
    {
        printf( "[Crack] Kernel isn't loaded (%d)\n", isKernelLoaded );
        // Same for kernel
        std::ifstream t_( "kernel" );
        std::stringstream buffer_;
        buffer_ << t_.rdbuf();
        auto str_ = buffer_.str();
        std::string* p_ = &str_;
        std::string pb_ = std::string();
        pb_.resize( p_->size() );
        // decode data
        typedefs::o_Cobalt_Base64( (__int64)&pb_, p_ );
        typedefs::o_Cobalt_StateChange( (char*) "[Cracked by Theo.#1337]\nStop pasting and ratting\nF2 to load kernel", 7 );

        printf( "[Crack] Decoding b64 for kernel done!\nPress F2 to load kernel\n" );
        while ( !GetAsyncKeyState( VK_F2 ) )
            Sleep(10); // cpu will thnx you 

        // map driver
        typedefs::o_KDMapper_Run(&pb_);
        printf( "[Crack] Loaded kernel !\n" );
    }
    printf( "[Crack] Decoding base64 for usermode\n" );
    typedefs::o_Cobalt_Base64( (__int64) &pb, p );
    
    printf( "[Crack] Waiting for WZ\n" );
    typedefs::o_Cobalt_StateChange( (char*) "[Cracked by Theo.#1337]\nStop pasting and ratting\nopen wz ", 7 );

    while ( !Utils::FindProcessId( L"ModernWarfare.exe" ) )
        Sleep( 50 );
    typedefs::o_Cobalt_StateChange( (char*) "[Cracked by Theo.#1337]\nStop pasting and ratting\nPress F1 to inject", 7 );

    printf( "[Crack] Press F1 whenever you wanna inject\n" );
    while ( !GetAsyncKeyState( VK_F1 ) )
        Sleep( 50 );

    typedefs::o_Cobalt_MapAndRun( Utils::FindProcessId( L"ModernWarfare.exe" ), &pb );
}

// WHY? 
/* 
*
*   We use that as a way for the software to load our dll before the code execute,
*   by rebuilding import table with our dll. (CFF EXPLORER)
*
*/
extern "C" __declspec(dllexport) int Lol_ImportMe( int code, WPARAM wParam, LPARAM lParam ) {
    return CallNextHookEx( NULL, code, wParam, lParam );
}
#include <thread>
void start()
{
    printf( "Cracked by Theo.#1337 #StopPastingPls\n\n\n" );
    HookFunction( reinterpret_cast<PVOID*>(& typedefs::o_Cobalt_StateChange), hooked_fnc::hk_Cobalt_StateChange );
    HookFunction( reinterpret_cast<PVOID*>(&typedefs::o_Cobalt_Base64), hooked_fnc::hk_Cobalt_Base64 );
    HookFunction( reinterpret_cast<PVOID*>(&typedefs::o_KDMapper_Run), hooked_fnc::hk_kdmapper );
    HookFunction( reinterpret_cast<PVOID*>(&typedefs::o_connect), hooked_fnc::hk_connect );
    HookFunction( reinterpret_cast<PVOID*>(&typedefs::o_ImGuiBegin), hooked_fnc::hk_ImGuiBegin );
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RunCrack, 0, 0, 0 );
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        AllocConsole();
        freopen( "CONOUT$", "w", stdout );
        start();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

