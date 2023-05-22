#include "catandroidassetmanager.hpp"
#include "SDL.h"

JNIenv *cat::pjnienv {};
AAssetManager *cat::asset::pmanager {};

void cat::init() {
    JNIEnv *pjnienv {(JNIEnv*) SDL_AndroidGetJNIEnv()};
    jobject activy {(jobject) SDL_AndroidGetActivity()};
    jclass activyclass {pjnienv->GetObjectClass(activy)};

    jmethodID activycallgetassets {pjnienv->GetMethodID(activyclass, "getAssets", "()Landroid/content/res/AssetManager;")};
    jobject assetmanager {pjnienv->CallObjectMethod(activy, activycallgetassets)};

    AAssetManager *passetmanager {AAssetManager_fromJava(pjnienv, assetmanager)};
    AAsset *passet {};
}