#pragma once
#include <SKSE/SKSE.h>
#include "SKSE/Interfaces.h"
#include <common/ITypes.h>
// Interface code based on https://github.com/adamhynek/higgs

namespace SkyrimVRESLPluginAPI
{
	constexpr const auto SkyrimVRESLPluginName = "SkyrimVRESL";
	// A message used to fetch SkyrimVRESL's interface
	struct SkyrimVRESLMessage
	{
		enum : uint32_t
		{
			kMessage_GetInterface = 0xeacb2bef
		};  // Randomly generated
		void* (*GetApiFunction)(unsigned int revisionNumber) = nullptr;
	};

	struct TESFileCollection
	{
	public:
		// members
        RE::BSTArray<RE::TESFile*> files;       // 00
        RE::BSTArray<RE::TESFile*> smallFiles;  // 18
	};
    // If that is a strict binary concern, then old code would need to be used.
    // STATIC_ASSERT(sizeof(TESFileCollection) == 0x30);

	// Returns an ISkyrimVRESLInterface001 object compatible with the API shown below
	// This should only be called after SKSE sends kMessage_PostLoad to your plugin
	struct ISkyrimVRESLInterface001;
    ISkyrimVRESLInterface001* GetSkyrimVRESLInterface001(const SKSE::PluginHandle& pluginHandle, const SKSE::MessagingInterface* messagingInterface);

	// This object provides access to SkyrimVRESL's mod support API
	struct ISkyrimVRESLInterface001
	{
		// Gets the SkyrimVRESL build number
		virtual unsigned int GetBuildNumber() = 0;

		/// @brief Get the SSE compatible TESFileCollection for SkyrimVR.
		/// This should be called after kDataLoaded to ensure it's been populated.
		/// @return Pointer to TESFileCollection CompiledFileCollection.
		const virtual TESFileCollection* GetCompiledFileCollection() = 0;
	};

// Converts the lower bits of a FormID to a full FormID depending on plugin type
static inline UInt32 GetFullFormID(const RE::TESFile* modInfo, UInt32 formLower)
{
	// Use modIndex of 0xFE as check for light plugin to determine proper form ID composition
    // return (modInfo->modIndex != 0xFE) ? UInt32(modInfo->modIndex) << 24 | (formLower & 0xFFFFFF) : 0xFE000000 | (UInt32(modInfo->lightIndex) << 12) | (formLower & 0xFFF);
    if (!modInfo) { // Explicitely checked to avoid crash
        return 0;
    }

    if (modInfo->IsLight()) {
        return (modInfo->GetPartialIndex() << 12) |
               (formLower & 0xFFF);
    }

    return (modInfo->GetPartialIndex() << 24) |
           (formLower & 0xFFFFFF);
}

const RE::TESFile* LookupAllLoadedModByName(const char* modName);
const RE::TESFile* LookupLoadedLightModByName(const char* modName);
}  // namespace SkyrimVRESLPluginAPI
extern SkyrimVRESLPluginAPI::ISkyrimVRESLInterface001* g_SkyrimVRESLInterface;
