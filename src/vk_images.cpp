#include <vk_images.h>
#include <vk_initializers.h>

// GPUs store their data in different formats for different uses, for example different format for reading and writing.
// VkImageLayout describes the current format.
// When using draw commands we need the ImageLayout in a writeable format, and when outputting to swapchain we need another format.
// Here we are using the synchronization2 extension from Vulkan 1.3
void vkutil::transition_image(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout) {
    VkImageMemoryBarrier2 imageBarrier {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .pNext = nullptr,
        // Wait for ALL previous pipeline stages to finish
        .srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        // Wait for all memory writes to complete and be visible
        .srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT,
        // Block ALL subsequent pipeline stages until the barrier completes
        .dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        // Ensure subsequent reads/writes see the completed writes
        .dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT,
        .oldLayout = currentLayout,
        .newLayout = newLayout,
    };

    VkImageAspectFlags aspectMask = 
        (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

    imageBarrier.subresourceRange = vkinit::image_subresource_range(aspectMask);
    imageBarrier.image = image;

    VkDependencyInfo depInfo {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &imageBarrier,
    };

    vkCmdPipelineBarrier2(cmd, &depInfo);
}