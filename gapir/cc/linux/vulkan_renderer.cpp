/*
 * Copyright (C) 2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gapir/cc/vulkan_gfx_api.h"
#include "gapir/cc/vulkan_renderer.h"

namespace gapir {
namespace {

class VulkanRendererImpl : public VulkanRenderer {
public:
    VulkanRendererImpl();
    virtual ~VulkanRendererImpl() override;

    virtual Api* api() override;

    virtual bool isValid() override;
private:
    Vulkan mApi;
};

VulkanRendererImpl::VulkanRendererImpl() {
    mApi.resolve();
    // Create a dummy instance renderer that never gets cleaned up.
    // This works around some driver bugs.
    // See https://github.com/google/gapid/issues/1899
    auto create_info = Vulkan::VkInstanceCreateInfo{
        Vulkan::VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        nullptr,
        0,
        nullptr,
        0,
        nullptr,
        0,
        nullptr
    };
    Vulkan::VkInstance inst;
    mApi.mFunctionStubs.vkCreateInstance(&create_info, nullptr, &inst);
}

VulkanRendererImpl::~VulkanRendererImpl() {
}

Api* VulkanRendererImpl::api() {
  return &mApi;
}

bool VulkanRendererImpl::isValid() {
 return mApi.mFunctionStubs.vkCreateInstance != nullptr;
}

} // anonymous namespace

VulkanRenderer* VulkanRenderer::create() {
    return new VulkanRendererImpl();
}


}  // namespace gapir
