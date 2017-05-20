#include "thunks.h"

namespace Thunks {
	ProcessCommandFP ProcessCommand = nullptr;
	OnServerErrorFP OnServerError = nullptr;
	OnPluginCommandFP OnPluginCommand = nullptr;
}