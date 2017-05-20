#include "mono_ts3func.h"
#include <ts3_functions.h>
#include <teamspeak/public_errors.h>
#include "container.h"
#include <mono/jit/jit.h>
#include <teamspeak/public_rare_definitions.h>
#include <functional>
#include "plat.h"

#define RETURNCODE_BUFSIZE 128
extern char returnCode[RETURNCODE_BUFSIZE]; //ToDo: allow plugin to use distinct returncodes

extern struct TS3Functions ts3Functions;
extern char* pluginID;

namespace MonoTS3Func {

	void RegisterFunctions() {
		ts3Functions.createReturnCode(pluginID, returnCode, RETURNCODE_BUFSIZE);

		mono_add_internal_call("DotSpeak.TS3Interface::GetServerConnectionHandlerList", GetServerConnectionHandlerList);
		mono_add_internal_call("DotSpeak.TS3Interface::GetCurrentServerConnectionHandler", GetCurrentServerConnectionHandler);
		mono_add_internal_call("DotSpeak.TS3Interface::SendPluginCommand", SendPluginCommand);
	}

	MonoArray* GetServerConnectionHandlerList() {
		uint64* result = nullptr;
		if (ts3Functions.getServerConnectionHandlerList(&result) != ERROR_ok || !result)
			return nullptr;

		size_t length = 0;
		for (uint64* counter = result; *counter != 0u; ++counter)
			++length;
		auto monoresult = mono_array_new(Container::CONT_domain, mono_get_uint64_class(), length);

		size_t idx = 0;
		for (uint64* counter = result; *counter != 0u; ++counter) {
			mono_array_set(monoresult, uint64, idx, *counter);
			++idx;
		}

		ts3Functions.freeMemory(result);
		return monoresult;
	}

	uint64_t GetCurrentServerConnectionHandler() {
		return ts3Functions.getCurrentServerConnectionHandlerID();
	}

	void SendPluginCommand(uint64_t serverConnectionHandlerID, MonoString* command, int32_t targetMode, MonoArray* targetIDs) {
		char* native_command = mono_string_to_utf8(command);
		anyID* native_targetIDs = nullptr;
		if(targetIDs) {
			auto elsize = mono_array_element_size(mono_object_get_class(reinterpret_cast<MonoObject*>(targetIDs)));
			if (elsize != sizeof(anyID))
				errmsg("managed size != native size");
			else
				native_targetIDs = reinterpret_cast<anyID*>(
					mono_array_addr_with_size(targetIDs, sizeof(anyID), 0));
		}
		
		ts3Functions.sendPluginCommand(serverConnectionHandlerID, pluginID, native_command, targetMode, native_targetIDs, returnCode);

		mono_free(native_command);
	}
}
