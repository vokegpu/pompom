#include "catandroidassetmanager.hpp"
#include "SDL.h"
#include "ekg/util/env.hpp"

JNIEnv *cat::pjnienv {};
AAssetManager *cat::asset::pmanager {};
std::string cat::asset::internalpath {};

void cat::init() {
    cat::pjnienv = (JNIEnv*) SDL_AndroidGetJNIEnv();


    jobject activy {(jobject) SDL_AndroidGetActivity()};
    jclass activyclass {cat::pjnienv->GetObjectClass(activy)};

    jmethodID activycallgetassets {cat::pjnienv->GetMethodID(activyclass, "getAssets", "()Landroid/content/res/AssetManager;")};
    jobject assetmanager {cat::pjnienv->CallObjectMethod(activy, activycallgetassets)};

    cat::asset::pmanager = AAssetManager_fromJava(cat::pjnienv, cat::pjnienv->NewGlobalRef(assetmanager));
    cat::asset::internalpath = std::string(SDL_AndroidGetInternalStoragePath());
}

std::string cat::asset::abs(std::string_view filename) {
    std::string abspath {cat::asset::internalpath};
    abspath += filename;
    return abspath;
}

bool cat::asset::extract(std::string_view assetdir, std::string_view assetfilename) {
    std::string path {}; path += assetdir; path += assetfilename;
    ekg::log() << path << " " << (cat::asset::pmanager == nullptr);

    AAsset *passet {AAssetManager_open(cat::asset::pmanager, path.c_str(), AASSET_MODE_BUFFER)};
    if (passet == nullptr) {
        return true;
    }

    FILE *pfilecheck {fopen(cat::asset::abs(assetfilename).c_str(), "w")};
    int8_t buf[BUFSIZ] {};

    size_t it {};
    while ((it = AAsset_read(passet, buf, BUFSIZ)) > 0) {
        fwrite(buf, it, 1, pfilecheck);
    }

    fclose(pfilecheck);
    AAsset_close(passet);

    return false;
}