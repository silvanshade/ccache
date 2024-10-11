// Copyright (C) 2020-2024 Joel Rosdahl and other contributors
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

#include <glaze/glaze.hpp>
#include <tl/expected.hpp>

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string_view>
#include <type_traits>
#include <vector>

namespace modules::depfiles {
class module_name_view
{
private:
  std::string_view repr;
  bool dotted = false;

  friend glz::detail::from<glz::JSON, modules::depfiles::module_name_view>;

public:
  module_name_view() noexcept;
  module_name_view(std::string_view view) noexcept;

  operator std::string_view() const noexcept;

  auto is_dotted() const noexcept -> bool;
};

static_assert(std::is_trivially_copyable_v<module_name_view>);

class path_view
{
private:
  std::string_view repr;

  friend glz::detail::from<glz::JSON, modules::depfiles::path_view>;

public:
  path_view() noexcept;
  path_view(std::string_view view) noexcept;

  operator std::string_view() const noexcept;
  explicit operator std::filesystem::path() const;
};

static_assert(std::is_trivially_copyable_v<path_view>);

enum class LookupMethod { ByName, IncludeAngle, IncludeQuote };

static_assert(std::is_trivially_copyable_v<LookupMethod>);

class ProvidedModuleDesc
{
public:
  std::optional<path_view> source_path;
  std::optional<path_view> compiled_module_path;
  std::optional<bool> unique_on_source_path = false;
  module_name_view logical_name;
  std::optional<bool> is_interface = true;
};

static_assert(std::is_trivially_copyable_v<ProvidedModuleDesc>);

class RequiredModuleDesc
{
public:
  std::optional<path_view> source_path;
  std::optional<path_view> compiled_module_path;
  std::optional<bool> unique_on_source_path = false;
  module_name_view logical_name;
  std::optional<LookupMethod> lookup_method = LookupMethod::ByName;
};

static_assert(std::is_trivially_copyable_v<RequiredModuleDesc>);

class DepInfo
{
  class Repr
  {
  public:
    std::optional<path_view> work_directory;
    std::optional<path_view> primary_output;
    std::optional<std::vector<path_view>> outputs;
    std::optional<std::vector<ProvidedModuleDesc>> provides;
    std::optional<std::vector<RequiredModuleDesc>> requires_;
  };

  friend glz::meta<Repr>;
  friend glz::detail::from<glz::JSON, modules::depfiles::DepInfo>;

  Repr repr;

  // Mark copy-operations private
  DepInfo(DepInfo const&) noexcept = default;
  DepInfo& operator=(DepInfo const&) noexcept = default;

public:
  DepInfo() noexcept = default;
  DepInfo(DepInfo&&) noexcept = default;
  DepInfo& operator=(DepInfo&&) noexcept = default;

  auto
  work_directory(this auto&& self) noexcept -> auto&&
  {
    return std::forward<decltype(self)>(self).repr.work_directory;
  }

  auto
  primary_output(this auto&& self) noexcept -> auto&&
  {
    return std::forward<decltype(self)>(self).repr.primary_output;
  }

  auto
  outputs(this auto&& self) noexcept -> auto&&
  {
    return std::forward<decltype(self)>(self).repr.outputs;
  }

  auto
  provides(this auto&& self) noexcept -> auto&&
  {
    return std::forward<decltype(self)>(self).repr.provides;
  }

  auto
  requires_(this auto&& self) noexcept -> auto&&
  {
    return std::forward<decltype(self)>(self).repr.requires_;
  }
};

static_assert(!std::is_trivially_copyable_v<DepInfo>);

static_assert(!std::is_copy_assignable_v<DepInfo>);
static_assert(!std::is_copy_constructible_v<DepInfo>);

static_assert(std::is_move_assignable_v<DepInfo>);
static_assert(std::is_move_constructible_v<DepInfo>);

class DepFile
{
  class Repr
  {
  public:
    std::uint32_t version;
    std::optional<std::uint32_t> revision = 0;
    std::vector<DepInfo> rules;
  };

  friend glz::meta<Repr>;
  friend glz::detail::from<glz::JSON, modules::depfiles::DepFile>;

  Repr repr;

  // Mark copy-operations private
  DepFile(DepFile const&) noexcept = default;
  DepFile& operator=(DepFile const&) noexcept = default;

public:
  class ParseError
  {
    glz::error_ctx repr;

    ParseError(glz::error_ctx&&);

    friend DepFile;

  public:
    using code = glz::error_code;

    operator bool() const noexcept;
    auto operator==(code err) const noexcept -> bool;
    auto format(std::string_view buffer) const noexcept -> std::string;
  };

  DepFile() noexcept = default;
  DepFile(DepFile&&) noexcept = default;
  DepFile& operator=(DepFile&&) noexcept = default;

  static auto parse(std::string_view buffer) noexcept
    -> tl::expected<DepFile, ParseError>;

  auto
  version(this auto&& self) noexcept -> auto&&
  {
    return std::forward<decltype(self)>(self).repr.version;
  }

  auto
  revision(this auto&& self) noexcept -> auto&&
  {
    return std::forward<decltype(self)>(self).repr.revision;
  }

  auto
  rules(this auto&& self) noexcept -> auto&&
  {
    return std::forward<decltype(self)>(self).repr.rules;
  }
};

static_assert(!std::is_trivially_copyable_v<DepFile>);

static_assert(!std::is_copy_assignable_v<DepFile>);
static_assert(!std::is_copy_constructible_v<DepFile>);

static_assert(std::is_move_assignable_v<DepFile>);
static_assert(std::is_move_constructible_v<DepFile>);

} // namespace modules::depfiles
