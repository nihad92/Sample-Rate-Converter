//
// Created by Nihad Ahmetovic on 2/16/21.
//
#ifndef SAMPLE_CONVERTER_ANDROID_HANDLE_H
#define SAMPLE_CONVERTER_ANDROID_HANDLE_H

#include <jni.h>
#include <map>
#include <vector>
#include <string>

std::map<long, std::vector<jobject>> gRefs{};

jfieldID getHandleField(JNIEnv *env, jobject obj) {
    jclass c = env->GetObjectClass(obj);
    // J is the type signature for long:
    return env->GetFieldID(c, "nativeHandle", "J");
}

template<typename T>
T *getHandle(JNIEnv *env, jobject obj) {
    jlong handle = env->GetLongField(obj, getHandleField(env, obj));
    return reinterpret_cast<T *>(handle);
}

template<typename T>
void setHandle(JNIEnv *env, jobject obj, T *t) {
    jlong handle = reinterpret_cast<jlong>(t);
    env->SetLongField(obj, getHandleField(env, obj), handle);
}

jobject createGlobalReference(JNIEnv *env,jobject thiz, jobject obj) {
    jlong handle = env->GetLongField(thiz, getHandleField(env, thiz));
    auto gRef = env->NewGlobalRef(obj);

    if (gRefs.find(handle) == gRefs.end()) {
        auto vector = std::vector<jobject>();
        vector.push_back(gRef);
        gRefs.insert(std::pair<jlong, std::vector<jobject>>(handle, vector));
    } else {
        gRefs[handle].push_back(gRef);
    }
    return gRef;
}

void clearGlobalReferences(JNIEnv *env, jobject obj) {
    jlong handle = env->GetLongField(obj, getHandleField(env, obj));

    for (int i = 0; i < gRefs[handle].size(); ++i) {
        env->DeleteGlobalRef(gRefs[handle][i]);
    }
    gRefs.erase(handle);
}

#endif //SAMPLE_CONVERTER_ANDROID_HANDLE_H
