#pragma once

#include <git_version_structs.hpp>
#include <string>

namespace gitversion {

const GitInfo& GetInfo();

const std::string& GetVersionRepresentationString();

}  // namespace gitversion
