#elibwebrtc

Use the https://github.com/webrtc-sdk/libwebrtc

## Installation

```bash
Windows:
gn gen out-debug/Windows-x64 --args="target_os=\"win\" target_cpu=\"x64\" is_component_build=false is_clang=true is_debug=true rtc_use_h264=true ffmpeg_branding=\"Chrome\" rtc_include_tests=false rtc_build_examples=false libwebrtc_desktop_capture=true" --ide=vs2019

MaxOS:
export ARCH=x64
gn gen out/mac-$ARCH --args="target_os=\"mac\" target_cpu=\"$ARCH\" is_debug=false rtc_include_tests=false rtc_use_h264=true ffmpeg_branding=\"Chrome\" is_component_build=false use_rtti=true use_custom_libcxx=false rtc_enable_protobuf=false rtc_include_tests=false rtc_build_examples=false libwebrtc_desktop_capture=true" 
```