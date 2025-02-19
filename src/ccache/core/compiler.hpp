// Copyright (C) 2023-2024 Joel Rosdahl and other contributors
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

#include <ccache/util/assertions.hpp>
#include <ccache/util/filesystem.hpp>

#include <optional>
#include <string>
#include <string_view>

namespace fs = util::filesystem;

class Config;

class Compiler
{
public:
  enum class type {
    auto_guess,
    clang,
    clang_cl,
    gcc,
    icl,
    icx,
    icx_cl,
    msvc,
    nvcc,
    other
  };

  // TODO: Enable shorter enum syntax once project standard is C++20.
  // using enum type;

  struct Description;

  class Type
  {
    type m_type;

  public:
    [[nodiscard]] static auto guess(const fs::path& path) -> Type;
    [[nodiscard]] constexpr static auto parse(std::string_view name) noexcept
      -> Type;

    constexpr Type(type type) noexcept;

    constexpr explicit operator std::string() const noexcept;
    constexpr operator std::string_view() const noexcept;
    constexpr operator type() const noexcept;

    [[nodiscard]] constexpr auto describe() const noexcept
      -> const Description&;
  };

  class FileExt
  {
    std::string_view m_repr;

  public:
    constexpr FileExt(const char* repr) noexcept;
    constexpr FileExt(std::string_view repr) noexcept;

    constexpr explicit operator std::string() const noexcept;
    constexpr operator std::string_view() const noexcept;
  };

  struct FileExts
  {
    std::optional<FileExt> binary_module_interface;
    std::optional<FileExt> dynamic_dependency_info;
    FileExt object;
    std::optional<FileExt> precompiled_header;
  };

  struct Description
  {
    Type type;
    std::string_view name;
    FileExts file_exts;
  };

private:
  Type m_type;
  std::string m_name;
  FileExts m_file_exts;

  constexpr Compiler(Description desc) noexcept;

public:
  constexpr Compiler(type type) noexcept;

  constexpr operator Type() const noexcept;
  constexpr operator type() const noexcept;

  // TODO: simplify accessors with deducing-this once project standard is C++23.

  [[nodiscard]] constexpr auto type() const noexcept -> const Type&;
  [[nodiscard]] constexpr auto type() noexcept -> Type&;

  [[nodiscard]] constexpr auto name() const noexcept -> const std::string&;
  [[nodiscard]] constexpr auto name() noexcept -> std::string&;

  [[nodiscard]] constexpr auto file_exts() const noexcept -> const FileExts&;
};

constexpr Compiler::FileExt::FileExt(const char* repr) noexcept
  : m_repr(std::string_view(repr))
{
}

constexpr Compiler::FileExt::FileExt(std::string_view repr) noexcept
  : m_repr(repr)
{
}

constexpr auto
Compiler::Type::parse(std::string_view name) noexcept -> Type
{
  if (name == "clang") {
    return Compiler::type::clang;
  } else if (name == "clang-cl") {
    return Compiler::type::clang_cl;
  } else if (name == "gcc") {
    return Compiler::type::gcc;
  } else if (name == "icl") {
    return Compiler::type::icl;
  } else if (name == "icx") {
    return Compiler::type::icx;
  } else if (name == "icx-cl") {
    return Compiler::type::icx_cl;
  } else if (name == "msvc") {
    return Compiler::type::msvc;
  } else if (name == "nvcc") {
    return Compiler::type::nvcc;
  } else if (name == "other") {
    return Compiler::type::other;
  } else {
    // Allow any unknown name for forward compatibility.
    return Compiler::type::auto_guess;
  }
}

constexpr Compiler::Type::Type(enum type type) noexcept : m_type(type)
{
}

constexpr Compiler::Type::
operator std::string() const noexcept
{
  return std::string(this->operator std::string_view());
}

constexpr Compiler::Type::
operator std::string_view() const noexcept
{
  if (m_type == Compiler::type::auto_guess) {
    return "auto";
  } else {
    return describe().name;
  }
}

constexpr Compiler::Type::
operator enum Compiler::type() const noexcept
{
  return m_type;
}

constexpr Compiler::FileExt::
operator std::string() const noexcept
{
  return std::string(this->operator std::string_view());
}

constexpr Compiler::FileExt::
operator std::string_view() const noexcept
{
  return m_repr;
}

constexpr Compiler::Compiler(Description desc) noexcept
  : m_type(desc.type),
    m_name(desc.name),
    m_file_exts(desc.file_exts)
{
}

constexpr Compiler::Compiler(enum type type) noexcept
  : Compiler(Type(type).describe())
{
}

// clang-format off
namespace descriptions {

inline static constexpr Compiler::Description auto_guess = {
  .type = Compiler::type::auto_guess,
  .name = {},
  .file_exts = {
    .binary_module_interface = {},
    .dynamic_dependency_info = {},
    .object = ".o",
    .precompiled_header = ".gch"
  }
};

inline static constexpr Compiler::Description clang = {
  .type = Compiler::type::clang,
  .name = "clang",
  .file_exts = {
    .binary_module_interface = {},
    .dynamic_dependency_info = {},
    .object = ".o",
    .precompiled_header = ".gch"
  }
};

inline static constexpr Compiler::Description clang_cl = {
  .type = Compiler::type::clang_cl,
  .name = "clang-cl",
  .file_exts = {
    .binary_module_interface = {},
    .dynamic_dependency_info = {},
    .object = ".obj",
    .precompiled_header = ".pch"
  }
};

inline static constexpr Compiler::Description gcc = {
  .type = Compiler::type::gcc,
  .name = "gcc",
  .file_exts = {
    .binary_module_interface = {},
    .dynamic_dependency_info = {},
    .object = ".o",
    .precompiled_header = ".gch"
  }
};

inline static constexpr Compiler::Description icl = {
  .type = Compiler::type::icl,
  .name = "icl",
  .file_exts = {
    .binary_module_interface = {},
    .dynamic_dependency_info = {},
    .object = ".obj",
    .precompiled_header = ".pch"
  }
};

inline static constexpr Compiler::Description icx = {
  .type = Compiler::type::icx,
  .name = "icx",
  .file_exts = {
    .binary_module_interface = {},
    .dynamic_dependency_info = {},
    .object = ".obj",
    .precompiled_header = ".pch"
  }
};

inline static constexpr Compiler::Description icx_cl = {
  .type = Compiler::type::icx_cl,
  .name = "icx-cl",
  .file_exts = {
    .binary_module_interface = {},
    .dynamic_dependency_info = {},
    .object = ".obj",
    .precompiled_header = ".pch"
  }
};

inline static constexpr Compiler::Description msvc = {
  .type = Compiler::type::msvc,
  .name = "msvc",
  .file_exts = {
    .binary_module_interface = {},
    .dynamic_dependency_info = {},
    .object = ".obj",
    .precompiled_header = ".pch"
  }
};

inline static constexpr Compiler::Description nvcc = {
  .type = Compiler::type::nvcc,
  .name = "nvcc",
  .file_exts = {
    .binary_module_interface = {},
    .dynamic_dependency_info = {},
    .object = ".o",
    .precompiled_header = ".gch"
  }
};

inline static constexpr Compiler::Description other = {
  .type = Compiler::type::other,
  .name = "other",
  .file_exts = {
    .binary_module_interface = {},
    .dynamic_dependency_info = {},
    .object = ".o",
    .precompiled_header = ".gch"
  }
};

} // namespace description
// clang-format on

constexpr auto
Compiler::Type::describe() const noexcept -> const Description&
{
  switch (m_type) {
  case Compiler::type::auto_guess:
    return descriptions::auto_guess;
  case Compiler::type::clang:
    return descriptions::clang;
  case Compiler::type::clang_cl:
    return descriptions::clang_cl;
  case Compiler::type::gcc:
    return descriptions::gcc;
  case Compiler::type::icl:
    return descriptions::icl;
  case Compiler::type::icx:
    return descriptions::icx;
  case Compiler::type::icx_cl:
    return descriptions::icx_cl;
  case Compiler::type::msvc:
    return descriptions::msvc;
  case Compiler::type::nvcc:
    return descriptions::nvcc;
  case Compiler::type::other:
    return descriptions::other;
  default:
    ASSERT(false);
  }
}

constexpr Compiler::
operator Type() const noexcept
{
  return Type(this->operator enum type());
}

constexpr Compiler::
operator enum type() const noexcept {
  return m_type;
}

constexpr auto
Compiler::type() const noexcept -> const Type&
{
  return m_type;
}

constexpr auto
Compiler::type() noexcept -> Type&
{
  return m_type;
}

constexpr auto
Compiler::name() const noexcept -> const std::string&
{
  return m_name;
}

constexpr auto
Compiler::name() noexcept -> std::string&
{
  return m_name;
}

constexpr auto
Compiler::file_exts() const noexcept -> const FileExts&
{
  return m_file_exts;
}
