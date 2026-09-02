#include <utility>
#include "util/to_underlying.hpp"
#include "SkyrimVRESLAPI.h"
#include "RE/T/TESDataHandler.h"
// Interface code based on https://github.com/adamhynek/higgs

// Stores the API after it has already been fetched
SkyrimVRESLPluginAPI::ISkyrimVRESLInterface001* g_SkyrimVRESLInterface = nullptr;

// Fetches the interface to use from SkyrimVRESL
SkyrimVRESLPluginAPI::ISkyrimVRESLInterface001* SkyrimVRESLPluginAPI::GetSkyrimVRESLInterface001(const SKSE::PluginHandle& pluginHandle, const SKSE::MessagingInterface* messagingInterface)
{
	// If the interface has already been fetched, rturn the same object
	if (g_SkyrimVRESLInterface) {
		return g_SkyrimVRESLInterface;
	}

	// Dispatch a message to get the plugin interface from SkyrimVRESL
	SkyrimVRESLMessage message;
    messagingInterface->Dispatch(SkyrimVRESLMessage::kMessage_GetInterface, (void*)&message, sizeof(SkyrimVRESLMessage*), SkyrimVRESLPluginName);
	if (!message.GetApiFunction) {
		return nullptr;
	}

	// Fetch the API for this version of the SkyrimVRESL interface
	g_SkyrimVRESLInterface = static_cast<ISkyrimVRESLInterface001*>(message.GetApiFunction(1));
	return g_SkyrimVRESLInterface;
}

const RE::TESFile* SkyrimVRESLPluginAPI::LookupAllLoadedModByName(const char* modName)
{
    RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();
	if (dataHandler)
	{
		if (!g_SkyrimVRESLInterface)
		{
			return dataHandler->LookupLoadedModByName(modName);
		}
		else
		{
            const RE::TESFile* modInfo = dataHandler->LookupLoadedModByName(modName);
			if (modInfo == nullptr)
			{
				modInfo = SkyrimVRESLPluginAPI::LookupLoadedLightModByName(modName);
			}
			return modInfo;
		}
	}
	return nullptr;
}

const RE::TESFile* SkyrimVRESLPluginAPI::LookupLoadedLightModByName(const char* modName)
{
	if (!g_SkyrimVRESLInterface)
	{
        RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();
        if (dataHandler)
		{
			return dataHandler->LookupLoadedModByName(modName);
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		const SkyrimVRESLPluginAPI::TESFileCollection* fileCollection = g_SkyrimVRESLInterface->GetCompiledFileCollection();
		if (fileCollection != nullptr)
		{
			for (auto* smallFile : fileCollection->smallFiles)
			{
				if (smallFile != nullptr)
				{
					int modNameLength = strlen(modName);
                    if (modNameLength == smallFile->GetFilename().length() && _strnicmp(smallFile->GetFilename().data(), modName, modNameLength) == 0)
					{
						return smallFile;
					}
				}
			}
		}
		return nullptr;
	}
}
