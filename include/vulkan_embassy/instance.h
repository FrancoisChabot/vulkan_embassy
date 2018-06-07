#ifndef VULKAN_EMBASSY_INSTANCE_INCLUDED_H
#define VULKAN_EMBASSY_INSTANCE_INCLUDED_H

#include "vulkan_embassy/errors.h"
#include "vulkan_embassy/expected.h"

#include <vulkan/vulkan.h>
#include <cstdint>

namespace vulkan_embassy {

template <typename RawT>
class raw_wrapper {
  static_assert(std::is_pod<RawT>::value, "");

 protected:
  RawT raw_;

 public:
  raw_wrapper() = default;
  explicit raw_wrapper(RawT const& v) : raw_(v) {}

  RawT& raw() { return raw_; }
  RawT const& raw() const { return raw_; }
};

class applicationInfo : public raw_wrapper<VkApplicationInfo> {
 public:
  applicationInfo() {
    raw_.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    raw_.pNext = nullptr;
    raw_.apiVersion = VK_API_VERSION_1_0;
    raw_.pApplicationName = nullptr;
    raw_.applicationVersion = 0;
    raw_.pEngineName = nullptr;
    raw_.engineVersion = 0;
  }

  void setApplicationInfo(const char* name, std::uint32_t version) {
    raw_.pApplicationName = name;
    raw_.applicationVersion = version;
  }

  void setEngineInfo(const char* name, std::uint32_t version) {
    raw_.pEngineName = name;
    raw_.engineVersion = version;
  }
};

class instanceCreateInfo : public raw_wrapper<VkInstanceCreateInfo> {
 public:
  instanceCreateInfo() {
    raw_.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    raw_.pNext = nullptr;
    raw_.flags = 0;
    raw_.pApplicationInfo = nullptr;
    raw_.enabledLayerCount = 0;
    raw_.ppEnabledLayerNames = nullptr;
    raw_.enabledExtensionCount = 0;
    raw_.ppEnabledExtensionNames = nullptr;
  }

  VkInstanceCreateInfo& raw() { return raw_; }
  VkInstanceCreateInfo const& raw() const { return raw_; }

  void setEnabledExtensions(std::uint32_t count, const char** names) {
    raw_.enabledExtensionCount = count;
    raw_.ppEnabledExtensionNames = names;
  }

  void setApplicationInfo(applicationInfo const& app) {
    raw_.pApplicationInfo = &app.raw();
  }
};

static std::uint32_t makeVersion(std::uint32_t major, std::uint32_t minor,
                                 std::uint32_t patch) {
  return VK_MAKE_VERSION(major, minor, patch);
}

class instance : public raw_wrapper<VkInstance> {
  instance(VkInstance const& raw) : raw_wrapper<VkInstance>(raw) { raw_ = raw; }

 public:
  instance() : raw_wrapper<VkInstance>(VK_NULL_HANDLE) {}
  instance(instance const&) = delete;
  instance& operator=(instance const&) = delete;

  instance(instance&& rhs) {
    raw_ = rhs.raw_;
    rhs.raw_ = VK_NULL_HANDLE;
  }

  instance& operator=(instance&& rhs) {
    raw_ = rhs.raw_;
    rhs.raw_ = VK_NULL_HANDLE;
  }

  ~instance() {
    if (raw_ != VK_NULL_HANDLE) {
      vkDestroyInstance(raw_, nullptr);
    }
  }

  friend expected<instance, CreateInstanceError> createInstance(
      instanceCreateInfo const& info);
};

expected<instance, CreateInstanceError> createInstance(
    instanceCreateInfo const& info) {
  VkInstance result;
  auto vk_err = vkCreateInstance(&info.raw(),
                                 nullptr,  // TODO: allocators
                                 &result);

  if (vk_err == VK_SUCCESS) {
    return instance(std::move(result));
  }

  return convertCreateInstanceError(vk_err);
}
}

#endif