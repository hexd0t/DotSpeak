#ifdef _WIN32
#pragma warning (disable : 4100)  /* Disable Unreferenced parameter warning */
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"
#include "ts3_functions.h"
#include "plugin.h"
#include "plat.h"
#include "container.h"

#define PLUGIN_API_VERSION 22
#define RETURNCODE_BUFSIZE 128

char returnCode[RETURNCODE_BUFSIZE]{}; //ToDo: allow plugin to use distinct returncodes
struct TS3Functions ts3Functions {};
char* pluginID = NULL;


const char* ts3plugin_name() {
	static const char* name = Container::PluginInfoName(); //Since we are memleaking here, at least do it only once
	return name;
}

const char* ts3plugin_version() {
	static const char* version = Container::PluginInfoVersion();
	return version;
}

int ts3plugin_apiVersion() {
	return PLUGIN_API_VERSION;
}

const char* ts3plugin_author() {
    static const char* author = Container::PluginInfoAuthor();
	return author;
}

const char* ts3plugin_description() {
	static std::string* desc = nullptr;
	if (!desc) {
		desc = new std::string(Container::PluginInfoDesc());
		*desc += "\n\npowered by DotSpeak";
	}
    return desc->c_str();
}

void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
    ts3Functions = funcs;
}

int ts3plugin_init() {
    return Container::Init() ? 0 : 1;
	/* 0 = success, 1 = failure, -2 = failure but client will not show a "failed to load" warning */
	/* -2 is a very special case and should only be used if a plugin displays a dialog (e.g. overlay) asking the user to disable
	 * the plugin again, avoiding the show another dialog by the client telling the user the plugin failed to load.
	 * For normal case, if a plugin really failed to load because of an error, the correct return value is 1. */
}

void ts3plugin_shutdown() {
	Container::MonoShutdown();

	if(pluginID) {
		free(pluginID);
		pluginID = nullptr;
	}
}

int ts3plugin_offersConfigure() {
	/*
	 * Return values:
	 * PLUGIN_OFFERS_NO_CONFIGURE         - Plugin does not implement ts3plugin_configure
	 * PLUGIN_OFFERS_CONFIGURE_NEW_THREAD - Plugin does implement ts3plugin_configure and requests to run this function in an own thread
	 * PLUGIN_OFFERS_CONFIGURE_QT_THREAD  - Plugin does implement ts3plugin_configure and requests to run this function in the Qt GUI thread
	 */
	return PLUGIN_OFFERS_NO_CONFIGURE; 
}

void ts3plugin_configure(void* handle, void* qParentWidget) {
    printf("PLUGIN: configure\n");
}

void ts3plugin_registerPluginID(const char* id) {
	const size_t sz = strlen(id) + 1;
	pluginID = new char[sz];
	memcpy(pluginID, id, sz);
}

const char* ts3plugin_commandKeyword() {
	static const char* keyword = Container::CommandKeyword();
	return keyword;
}

int ts3plugin_processCommand(uint64 serverConnectionHandlerID, const char* command) {
	return Container::ProcessCommand(serverConnectionHandlerID, command) ? 0 : 1;
}

int ts3plugin_onServerErrorEvent(uint64 serverConnectionHandlerID, const char* errorMessage, unsigned int error, const char* retCode, const char* extraMessage) {
	if (!retCode || strcmp(retCode, returnCode) != 0)
		return 0;
	
	int result = 0;

	if (Container::OnServerError(serverConnectionHandlerID, errorMessage, error, extraMessage))
		result = 1;

	if(result == 0 && (error == ERROR_ok || error == ERROR_ok_no_update)) //Tell TS3 not to "Error" on success, even if Plugin didn't catch it
		return 1;
	return result;
	/* 0: Client will continue handling this error (print to chat tab)
	* 1: Client will ignore this error, the plugin announces it has handled it */
}

void ts3plugin_onPluginCommandEvent(uint64 serverConnectionHandlerID, const char* pluginName, const char* pluginCommand) {
	if (!pluginName || strcmp(pluginName, GetFilenameWithoutExt().c_str()) != 0)
		return;
	Container::OnPluginCommand(serverConnectionHandlerID, pluginCommand);
}