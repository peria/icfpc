#pragma once

#include <memory>

template <typename T>
using Pointer = std::shared_ptr<T>;

template <class T, class U>
Pointer<T> As(const Pointer<U>& r) noexcept {
  return std::dynamic_pointer_cast<T>(r);
}
