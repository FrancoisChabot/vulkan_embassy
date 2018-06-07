#ifndef VULKAN_EMBASSY_ERRORS_INCLUDED_H
#define VULKAN_EMBASSY_ERRORS_INCLUDED_H

#include <vulkan/vulkan.h>
#include "vulkan_embassy/expected.h"

#define CreateInstanceError_table        \
  VKE_ERROR_ENTRY(OUT_OF_HOST_MEMORY)    \
  VKE_ERROR_ENTRY(OUT_OF_DEVICE_MEMORY)  \
  VKE_ERROR_ENTRY(INITIALIZATION_FAILED) \
  VKE_ERROR_ENTRY(LAYER_NOT_PRESENT)     \
  VKE_ERROR_ENTRY(EXTENSION_NOT_PRESENT) \
  VKE_ERROR_ENTRY(INCOMPATIBLE_DRIVER)

namespace vulkan_embassy {
enum class CreateInstanceError {
#define VKE_ERROR_ENTRY(name) name = VK_ERROR_##name,
  CreateInstanceError_table
#undef VKE_ERROR_ENTRY
      UNKNOWN
};

unexpected<CreateInstanceError> convertCreateInstanceError(VkResult result) {
  switch (result) {
#define VKE_ERROR_ENTRY(name) \
  case VK_ERROR_##name:       \
    return make_unexpected(CreateInstanceError::name);
    CreateInstanceError_table
#undef VKE_ERROR_ENTRY
        default : break;
  }
  return make_unexpected(CreateInstanceError::UNKNOWN);
}
}

#undef CreateInstanceError_table

#endif
