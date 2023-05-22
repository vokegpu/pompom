#ifndef CAT_ANDROID_ASSET_MANAGER_H
#define CAT_ANDROID_ASSET_MANAGER_H

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

namespace cat {
    extern JNIEnv *pjnienv;

    struct asset {
        static AAssetManager *pmanager;
        static void extract();
    };

    void init();
}

#endif