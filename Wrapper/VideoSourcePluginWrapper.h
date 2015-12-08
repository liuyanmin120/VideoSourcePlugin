/**
 * John Bradley (jrb@turrettech.com)
 */
#pragma once

#include "OBSApi.h"

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

// add by liuym
typedef void(*STARTSTOPSTREAM_CALLBACK)(void* pVoid);

EXTERN_DLL_EXPORT bool LoadPlugin();
EXTERN_DLL_EXPORT void UnloadPlugin();
EXTERN_DLL_EXPORT void OnStartStream();
EXTERN_DLL_EXPORT void OnStopStream();
EXTERN_DLL_EXPORT CTSTR GetPluginName();
EXTERN_DLL_EXPORT CTSTR GetPluginDescription();
// add by liuym
EXTERN_DLL_EXPORT void SetStreamListen(STARTSTOPSTREAM_CALLBACK pCallback);

typedef bool (*LOADPLUGIN_PROC)();
typedef void (*UNLOADPLUGIN_PROC)();
typedef void (*ONSTARTSTREAM_PROC)();
typedef void (*ONSTOPSTREAM_PROC)();
typedef CTSTR (*GETPLUGINNAME_PROC)();
typedef CTSTR (*GETPLUGINDESCRIPTION_PROC)();
// add by liuym
typedef void* (*GETVLCOBJECT_PROC)();