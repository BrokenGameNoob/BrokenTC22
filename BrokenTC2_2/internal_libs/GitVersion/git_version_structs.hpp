#pragma once

#include <cstdint>
#include <sstream>
#include <string>

namespace gitversion {

/* Version structs */

struct Version {
  int32_t major{};
  int32_t minor{};
  int32_t patch{};
};

inline bool operator==(const Version& r, const Version& d) {
  return r.major == d.major && r.minor == d.minor && r.patch == d.patch;
}
inline bool operator>(const Version& r, const Version& d) {
  if (r.major > d.major)
    return true;
  else if (r.major < d.major)
    return false;

  if (r.minor > d.minor)
    return true;
  else if (r.minor < d.minor)
    return false;

  if (r.patch > d.patch) return true;
  return false;
}
inline bool operator>=(const Version& r, const Version& d) {
  return r == d || r > d;
}
inline bool operator<(const Version& r, const Version& d) {
  return !(r >= d);
}

inline std::string to_string(const Version& v, bool format_as_tag = false) {
  std::stringstream stream{};
  if (format_as_tag) {
    stream << "v";
  }
  stream << v.major << "." << v.minor << "." << v.patch;
  return stream.str();
}

/* Git info structs */

struct GitInfo {
  Version last_known_version{};
  bool is_release{};  /* True if current tag is a version vX.X.X */
  bool is_modified{}; /* True if the HEAD is in a detached state (diff output not null) */
  std::string current_commit{};
  std::string current_tag{};
};

inline std::string GetVersionRepresentationString(const GitInfo& git_info) {
  std::string release_string{git_info.is_release ? to_string(git_info.last_known_version, true) : ""};
  std::string non_release{
      git_info.is_release ? ""
                          : git_info.current_commit +
                                (!git_info.current_tag.empty() ? " ([" + git_info.current_tag + "] from " : " (from ") +
                                to_string(git_info.last_known_version, true) + ")"};
  std::string modified_string{git_info.is_modified ? "-modified" : ""};
  return release_string + non_release + modified_string;
}

}  // namespace gitversion
