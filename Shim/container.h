#pragma once
#include <string>
#include <mono/metadata/loader.h>

namespace Container {
	void MonoInit();
	void MonoShutdown();
	const char* GetStaticFieldString(const char* fieldName);
	const char* PluginInfoName();
	const char* PluginInfoVersion();
	const char* PluginInfoAuthor();
	const char* PluginInfoDesc();
	const char* CommandKeyword();
	bool Init();

	extern MonoDomain* CONT_domain;

	//==================================================== Events
	bool ProcessCommand(uint64 serverConnectionHandlerID, const char* command);
	bool OnServerError(uint64 serverConnectionHandlerID, const char* errorMessage, unsigned int error, const char* extraMessage);
	void OnPluginCommand(uint64 serverConnectionHandlerID, const char* pluginCommand);
}
