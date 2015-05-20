APP_ABI := armeabi-v7a
APP_PLATFORM := android-11

NDK_TOOLCHAIN_VERSION := clang
#NDK_TOOLCHAIN_VERSION := 4.8

APP_STL := gnustl_static
APP_GNUSTL_FORCE_CPP_FEATURES := rtti exceptions
APP_CPPFLAGS += -std=c++11 -std=gnu++11
APP_CFLAGS += -D"BOOST_PP_ITERATION_FLAGS_0()"=0
APP_CFLAGS += -DIS_GOOGLE

# turn off noisy clang warnings
APP_CFLAGS += -Wno-deprecated-register -Wno-deprecated-declarations -Wno-switch -Wno-unknown-pragmas -Wno-comment

# 4.8 toolchain defaults to dwarf-4, not compatible to NDK yet
APP_CFLAGS += -g -gdwarf-2 -gstrict-dwarf