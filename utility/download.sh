#!/usr/bin/env bash

set -exo pipefail

TARGET=${TARGET:=glad/files}

download_file() {
	rm -f "$1"
	wget --quiet --show-progress -O "$1" "$2"
}

download_file "${TARGET}/egl.xml" https://raw.githubusercontent.com/KhronosGroup/EGL-Registry/main/api/egl.xml
download_file "${TARGET}/egl_angle_ext.xml" https://raw.githubusercontent.com/google/angle/main/scripts/egl_angle_ext.xml

download_file "${TARGET}/gl.xml" https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/main/xml/gl.xml
download_file "${TARGET}/gl_angle_ext.xml" https://raw.githubusercontent.com/google/angle/main/scripts/gl_angle_ext.xml

# These extensions are GLSL-only, so they don't end up in the Khronos XML
download_file "${TARGET}/glsl_exts.xml" 'https://www.uplinklabs.net/glsl_exts.xml'

download_file "${TARGET}/glx.xml" https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/main/xml/glx.xml

download_file "${TARGET}/wgl.xml" https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/main/xml/wgl.xml

download_file "${TARGET}/vk.xml" https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/xml/vk.xml

download_file "${TARGET}/khrplatform.h" https://raw.githubusercontent.com/KhronosGroup/EGL-Registry/main/api/KHR/khrplatform.h

download_file "${TARGET}/eglplatform.h" https://raw.githubusercontent.com/KhronosGroup/EGL-Registry/main/api/EGL/eglplatform.h

download_file "${TARGET}/vk_platform.h" https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/include/vulkan/vk_platform.h

download_file "${TARGET}/vulkan_video_codec_h264std.h" https://raw.githubusercontent.com/KhronosGroup/Vulkan-Headers/main/include/vk_video/vulkan_video_codec_h264std.h
download_file "${TARGET}/vulkan_video_codec_h264std_decode.h" https://raw.githubusercontent.com/KhronosGroup/Vulkan-Headers/main/include/vk_video/vulkan_video_codec_h264std_encode.h
download_file "${TARGET}/vulkan_video_codec_h264std_encode.h" https://raw.githubusercontent.com/KhronosGroup/Vulkan-Headers/main/include/vk_video/vulkan_video_codec_h264std_decode.h

download_file "${TARGET}/vulkan_video_codec_h265std.h" https://raw.githubusercontent.com/KhronosGroup/Vulkan-Headers/main/include/vk_video/vulkan_video_codec_h265std.h
download_file "${TARGET}/vulkan_video_codec_h265std_decode.h" https://raw.githubusercontent.com/KhronosGroup/Vulkan-Headers/main/include/vk_video/vulkan_video_codec_h265std_encode.h
download_file "${TARGET}/vulkan_video_codec_h265std_encode.h" https://raw.githubusercontent.com/KhronosGroup/Vulkan-Headers/main/include/vk_video/vulkan_video_codec_h265std_decode.h
