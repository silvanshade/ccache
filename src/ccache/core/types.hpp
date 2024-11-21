// Copyright (C) 2021-2022 Joel Rosdahl and other contributors
//
// See doc/AUTHORS.adoc for a complete list of contributors.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#pragma once

#include <cstdint>
#include <string>

class Config;

namespace core {

enum class CacheEntryType : uint8_t {
  result = 0,
  manifest = 1,
  cxx_module_ref = 2,
};

std::string to_string(CacheEntryType type);

enum class CompressionType : uint8_t {
  none = 0,
  zstd = 1,
};

int8_t compression_level_from_config(const Config& config);

CompressionType compression_type_from_config(const Config& config);

CompressionType compression_type_from_int(uint8_t type);

std::string to_string(CompressionType type);

} // namespace core
