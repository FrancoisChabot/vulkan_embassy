#define VULKAN_EMBASSY_USE_EXCEPTIONS
#include "vulkan_embassy/vulkan_embassy.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

namespace vke = vulkan_embassy;

void init_glfw() {
  glfwSetErrorCallback(
      [](int, const char* e) { std::cerr << "GLFW error: " << e << "\n"; });

  if (!glfwInit()) {
    throw std::runtime_error("glfw init failure");
  }

  if (!glfwVulkanSupported()) {
    throw std::runtime_error("glfw has no vulkan support");
  }
}

vke::instance init_vk() {
  vke::applicationInfo app_info;
  app_info.setApplicationInfo("hello_vulkan", vke::makeVersion(1, 0, 0));
  app_info.setEngineInfo("vke_demo", vke::makeVersion(1, 0, 0));

  uint32_t extensions_count = 0;
  const char** extensions =
      glfwGetRequiredInstanceExtensions(&extensions_count);
  if (!extensions) {
    throw std::runtime_error(
        "glfwGetRequiredInstanceExtensions failure, missing vulkan driver?");
  }

  vke::instanceCreateInfo instance_info;
  instance_info.setApplicationInfo(app_info);
  instance_info.setEnabledExtensions(extensions_count, extensions);

  auto result = vke::createInstance(instance_info);
  if (!result) {
    throw std::runtime_error("failed to create vk instance");
  }

  return std::move(*result);
}

int main() {
  init_glfw();
  vke::instance vk = init_vk();

  return 0;
}