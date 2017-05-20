#pragma once
#include <mono/metadata/reflection.h>

namespace Thunks {
	typedef MonoBoolean (*ProcessCommandFP) (MonoObject* instance, uint64_t serverConnectionHandlerID, MonoString* command, MonoException** ex);
	extern ProcessCommandFP ProcessCommand;

	typedef MonoBoolean(*OnServerErrorFP) (MonoObject* instance, uint64_t serverConnectionHandlerID, MonoString* msg, uint32_t error, MonoString* extraMsg, MonoException** ex);
	extern OnServerErrorFP OnServerError;

	typedef void(*OnPluginCommandFP) (MonoObject* instance, uint64_t serverConnectionHandlerID, MonoString* pluginCommand, MonoException** ex);
	extern OnPluginCommandFP OnPluginCommand;
}
