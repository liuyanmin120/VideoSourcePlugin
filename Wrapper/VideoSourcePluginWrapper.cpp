/**
* John Bradley (jrb@turrettech.com)
*/

#include "VideoSourcePluginWrapper.h"

#include <Ole2.h>

static HINSTANCE hinstDLL = 0;
static HMODULE hmodVspPlugin;
static HMODULE hmodLibVlc;
static HMODULE hmodLibVlcCore;
// add by liuym
static STARTSTOPSTREAM_CALLBACK StartStopStreamCallBack = 0;

static LOADPLUGIN_PROC InternalLoadPlugin = 0;
static UNLOADPLUGIN_PROC InternalUnloadPlugin = 0;
static ONSTARTSTREAM_PROC InternalOnStartStream = 0;
static ONSTOPSTREAM_PROC InternalOnStopStream = 0;
static GETPLUGINNAME_PROC InternalGetPluginName = 0;
static GETPLUGINDESCRIPTION_PROC InternalGetPluginDescription = 0;
// add by liuym
static GETVLCOBJECT_PROC InternalGetVlcObject = 0;

bool LoadPlugin()
{
    if (InternalLoadPlugin &&
        InternalUnloadPlugin &&
        InternalOnStartStream && 
        InternalOnStopStream &&
        InternalGetPluginName &&
        InternalGetPluginDescription) 
    {
        return InternalLoadPlugin();
    }

    return false;
}

void UnloadPlugin()
{
    InternalUnloadPlugin();
}

void OnStartStream()
{
	InternalGetVlcObject();
	if (StartStopStreamCallBack)
		StartStopStreamCallBack(InternalGetVlcObject());
    InternalOnStartStream();
}

void OnStopStream()
{
	if (StartStopStreamCallBack)
		StartStopStreamCallBack(NULL);
    InternalOnStopStream();
}

CTSTR GetPluginName()
{
    return InternalGetPluginName();
}

CTSTR GetPluginDescription()
{
    return InternalGetPluginDescription();
}

void SetStreamListen(STARTSTOPSTREAM_CALLBACK pCallback)
{
	StartStopStreamCallBack = pCallback;
}

BOOL CALLBACK DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    static bool isOleInitialized = false;
        
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        {
            isOleInitialized = OleInitialize(0) ? true : false;

            // order is important!
            hmodLibVlcCore = LoadLibrary(L".\\plugins\\MediaPlayer\\libvlccore.dll");
            hmodLibVlc = LoadLibrary(L".\\plugins\\MediaPlayer\\libvlc.dll");
        
            // main plugin dll
            hmodVspPlugin = LoadLibrary(L".\\plugins\\MediaPlayer\\MediaPlayer.dll"); 

            if (hmodVspPlugin != NULL) {
                InternalLoadPlugin = (LOADPLUGIN_PROC)GetProcAddress(hmodVspPlugin, "LoadPlugin");
                InternalUnloadPlugin = (UNLOADPLUGIN_PROC)GetProcAddress(hmodVspPlugin, "UnloadPlugin");
                InternalOnStartStream = (ONSTARTSTREAM_PROC)GetProcAddress(hmodVspPlugin, "OnStartStream");
                InternalOnStopStream = (ONSTOPSTREAM_PROC)GetProcAddress(hmodVspPlugin, "OnStopStream");
                InternalGetPluginName = (GETPLUGINNAME_PROC)GetProcAddress(hmodVspPlugin, "GetPluginName");
                InternalGetPluginDescription = (GETPLUGINDESCRIPTION_PROC)GetProcAddress(hmodVspPlugin, "GetPluginDescription");
				InternalGetVlcObject = (GETVLCOBJECT_PROC)GetProcAddress(hmodVspPlugin, "GetVlcObject");
            }
            break;
        }
    case DLL_PROCESS_DETACH:
        {
            if (isOleInitialized) {
                OleUninitialize();
            }

            if (hmodVspPlugin) FreeLibrary(hmodVspPlugin);
            if (hmodLibVlc) FreeLibrary(hmodLibVlc);
            if (hmodLibVlcCore) FreeLibrary(hmodLibVlcCore);
            break;
        }
    }
    return TRUE;
}
