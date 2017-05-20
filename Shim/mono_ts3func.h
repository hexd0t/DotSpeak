#pragma once
#include <mono/metadata/object.h>
namespace MonoTS3Func {
	void RegisterFunctions();

	MonoArray* GetServerConnectionHandlerList();
	uint64_t GetCurrentServerConnectionHandler();
	void SendPluginCommand(uint64_t serverConnectionHandlerID, MonoString* command, int32_t targetMode, MonoArray* targetIDs);
}
