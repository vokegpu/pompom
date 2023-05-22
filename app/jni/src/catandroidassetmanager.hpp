#ifndef CAT_ANDROID_ASSET_MANAGER_H
#define CAT_ANDROID_ASSET_MANAGER_H

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <iostream>

namespace cat {
    extern JNIEnv *pjnienv;

    struct asset {
    public:
        static AAssetManager *pmanager;
        static std::string internalpath;
        static std::string abs(std::string_view filename);
        static bool extract(std::string_view assetdir, std::string_view assetfilename);
    };

    void init();
}

#endif