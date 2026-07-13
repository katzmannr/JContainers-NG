#include "SKSE/SKSE.h"
#include "REL/Relocation.h"

SKSEPluginInfo(
    .Version = REL::Version{ 1, 0, 0, 0 },
    .Name = "JContainers"sv,
    .Author = ""sv,
    .SupportEmail = ""sv,
    .StructCompatibility = SKSE::StructCompatibility::Independent,
    .RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary,
    .MinimumSKSEVersion = REL::Version{ 0, 0, 0, 0 }
)
