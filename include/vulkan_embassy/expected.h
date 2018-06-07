#ifndef VULKAN_EMBASSY_EXPECTED_INCLUDED_H
#define VULKAN_EMBASSY_EXPECTED_INCLUDED_H

#include <cassert>
#include <utility>

namespace vulkan_embassy {

// This is meant to be an approximation of std::expected, as proposed in N4109,
// providing the minimum set of features
template <typename T>
struct unexpected {
  constexpr unexpected(unexpected const&) = default;
  constexpr unexpected(unexpected&&) = default;
  constexpr unexpected& operator=(unexpected const&) = default;
  constexpr unexpected& operator=(unexpected&&) = default;

  explicit constexpr unexpected(T v) : value(std::move(v)) {}

  T value;
};

template <typename T>
unexpected<T> make_unexpected(T v) {
  return unexpected<T>(std::move(v));
}

template <typename T, typename E>
class expected {
  using success_type = T;
  using failure_type = E;

  bool has_value_;

  struct dummy {};

  union {
    dummy dummy_;
    success_type success_;
    failure_type failure_;
  };

 public:
  expected(success_type v) : has_value_(true) { success_ = std::move(v); }

  expected(expected&& rhs) {
    has_value_ = rhs.has_value_;
    if (has_value_) {
      success_ = std::move(rhs.success_);
    } else {
      failure_ = std::move(rhs.failure_);
    }
  }

  expected(expected const& rhs) {
    has_value_ = rhs.has_value_;
    if (has_value_) {
      success_ = rhs.success_;
    } else {
      failure_ = rhs.failure_;
    }
  }

  expected(unexpected<failure_type> v) : has_value_(true) {
    failure_ = std::move(v.value);
  }

  ~expected() {
    if (has_value_) {
      success_.~success_type();
    } else {
      failure_.~failure_type();
    }
  }

  T& operator*() {
    assert(has_value_);

    return success_;
  }

  operator bool() const { return has_value_; }
};
}
#endif
