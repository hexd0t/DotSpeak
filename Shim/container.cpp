#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>
#include <ts3_functions.h>
#include "container.h"
#include "plat.h"
#include "Thunks.h"
#include "mono_ts3func.h"

#ifdef _WIN32
extern "C" {
	EXPORTDLL void InitMono();
}
#endif

extern char* pluginID;
extern struct TS3Functions ts3Functions;

namespace Container {

	static bool INIT = false;

	MonoDomain* CONT_domain = nullptr;
	MonoAssembly* CONT_assembly = nullptr;
	MonoImage* CONT_image = nullptr;
	MonoClass* CONT_pluginclass = nullptr;
	MonoVTable* CONT_pluginvtable = nullptr;
	MonoObject* CONT_plugininst = nullptr;
	MonoClass* CONT_baseclass = nullptr;
	MonoVTable* CONT_basevtable = nullptr;

	std::string getDLLPath() {
		std::string result(GetPluginsDir());
		result += GetFilenameWithoutExt() + "/plugin.dll";
		return result;
	}

	void MonoInit() {
		if (INIT)
			return;

		InitMono(); //Global, once per process init

		{ //create_appdomain requires mutable string :|
			auto tmpname = new char[GetFilename().size() + 1];
			memcpy(tmpname, GetFilename().c_str(), GetFilename().size() + 1);
			CONT_domain = mono_domain_create_appdomain(tmpname, nullptr);
			delete[] tmpname;
		}

		mono_domain_set(CONT_domain, true);
		auto dllPath = getDLLPath();

		CONT_assembly = mono_domain_assembly_open(CONT_domain, dllPath.c_str());
		if (!CONT_assembly)
			return;

		CONT_image = mono_assembly_get_image(CONT_assembly);

		CONT_pluginclass = mono_class_from_name(CONT_image, "Plugin", "Plugin");
		CONT_pluginvtable = mono_class_vtable(CONT_domain, CONT_pluginclass);
		mono_runtime_class_init(CONT_pluginvtable);

		CONT_baseclass = mono_class_get_parent(CONT_pluginclass);
		CONT_basevtable = mono_class_vtable(CONT_domain, CONT_baseclass);
		mono_runtime_class_init(CONT_basevtable);

		uint32_t version = 0;
		auto versionfield = mono_class_get_field_from_name(CONT_baseclass, "DotSpeakAPIVersion");
		if (versionfield && mono_type_get_type(mono_field_get_type(versionfield)) == MONO_TYPE_U4)
			mono_field_static_get_value(CONT_basevtable, versionfield, &version);
		if (version == 0 || version > 1)
			errmsg("Plugin requires newer DotSpeak, please update!");

		INIT = true;
	}

	void MonoShutdown() {
		/*if (!INIT)
			return;

		//mono_domain_unload(CONT_domain); //Exceptions out in mono 4.8 ...?

		INIT = false;*/
	}

	const char* GetStaticFieldString(const char* fieldName) {
		mono_domain_set(CONT_domain, true);
		auto field = mono_class_get_field_from_name(CONT_pluginclass, fieldName);
		if (!field) {
			return nullptr;
		}

		if (mono_type_get_type(mono_field_get_type(field)) != MONO_TYPE_STRING) {
			errmsg(std::string("Plugin::Plugin::") + fieldName + " is not a string!");
			return "Err";
		}

		MonoString *strval = nullptr;
		mono_field_static_get_value(CONT_pluginvtable, field, &strval);

		if (!strval) {
			errmsg(std::string("Plugin::Plugin::") + fieldName + " is not a static field!");
			return "Err";
		}

		auto result = mono_string_to_utf8(strval);
		return result;
	}

	const char* GetMandatoryStaticFieldString(const char* fieldName) {
		auto result = GetStaticFieldString(fieldName);
		if (result == nullptr) {
			errmsg(std::string("Plugin::Plugin does not contain valid '")+ fieldName+"' field!");
			return "Err";
		}
		return result;
	}

	const char* Container::PluginInfoName() {
		MonoInit();
		return GetMandatoryStaticFieldString("Name");
	}
	const char* PluginInfoVersion() {
		MonoInit();
		return GetMandatoryStaticFieldString("Version");
	}
	const char* PluginInfoAuthor() {
		MonoInit();
		return GetMandatoryStaticFieldString("Author");
	}
	const char* PluginInfoDesc() {
		MonoInit();
		return GetMandatoryStaticFieldString("Desc");
	}
	const char* CommandKeyword() {
		return GetStaticFieldString("CommandKeyword");
	}

	void CreateThunks() {
		auto method = mono_class_get_method_from_name(CONT_baseclass, "OnProcessCommand", 2);
		if(method)
			Thunks::ProcessCommand = static_cast<Thunks::ProcessCommandFP>(mono_method_get_unmanaged_thunk(method));
		method = mono_class_get_method_from_name(CONT_baseclass, "OnServerError", 4);
		if (method)
			Thunks::OnServerError = static_cast<Thunks::OnServerErrorFP>(mono_method_get_unmanaged_thunk(method));
		method = mono_class_get_method_from_name(CONT_baseclass, "OnPluginCommand", 2);
		if (method)
			Thunks::OnPluginCommand = static_cast<Thunks::OnPluginCommandFP>(mono_method_get_unmanaged_thunk(method));
	}

	bool Init() {
		mono_domain_set(CONT_domain, true);

		MonoTS3Func::RegisterFunctions();

		CONT_plugininst = mono_object_new(CONT_domain, CONT_pluginclass);
		auto method = mono_class_get_method_from_name(CONT_pluginclass, ".ctor", 0);
		if (!method) {
			errmsg("Cannot find constructor!");
			return false;
		}
		MonoObject* exception = nullptr;
		mono_runtime_invoke(method, CONT_plugininst, nullptr, &exception);
		if (exception) {
			auto excstr = mono_string_to_utf8(mono_object_to_string(exception, nullptr));
			errmsg(excstr);
			mono_free(excstr);
			return false;
		}

		CreateThunks();
		return true;
	}

	bool ProcessCommand(uint64 serverConnectionHandlerID, const char* command) {
		if(!Thunks::ProcessCommand) {
			//Message for ProcessCommand, since the plugin registered a keyword and clearly wants to process commands
			errmsg("Couldn't locate managed ProcessCommand event!"); 
			return false;
		}
		MonoException* ex = nullptr;
		auto result = Thunks::ProcessCommand(CONT_plugininst, serverConnectionHandlerID,
			mono_string_new(CONT_domain, command), &ex);

		if(ex) {
			auto excstr = mono_string_to_utf8(
				mono_object_to_string(reinterpret_cast<MonoObject*>(ex), nullptr));
			errmsg(excstr);
			mono_free(excstr);
			return false;
		}

		return result;
	}

	bool OnServerError(uint64 serverConnectionHandlerID, const char* errorMessage, unsigned int error, const char* extraMessage) {
		if (!Thunks::OnServerError) {
			return false;
		}
		MonoException* ex = nullptr;

		auto result = Thunks::OnServerError(CONT_plugininst, serverConnectionHandlerID, mono_string_new(CONT_domain, errorMessage),
			error, mono_string_new(CONT_domain, extraMessage), &ex);

		if (ex) {
			auto excstr = mono_string_to_utf8(
				mono_object_to_string(reinterpret_cast<MonoObject*>(ex), nullptr));
			errmsg(excstr);
			mono_free(excstr);
			return false;
		}

		return result;
	}

	void OnPluginCommand(uint64 serverConnectionHandlerID,  const char* pluginCommand) {
		if(!Thunks::OnPluginCommand) {
			return;
		}

		MonoException* ex = nullptr;

		Thunks::OnPluginCommand(CONT_plugininst, serverConnectionHandlerID, mono_string_new(CONT_domain, pluginCommand), &ex);

		if (ex) {
			auto excstr = mono_string_to_utf8(
				mono_object_to_string(reinterpret_cast<MonoObject*>(ex), nullptr));
			errmsg(excstr);
			mono_free(excstr);
		}
	}
}
