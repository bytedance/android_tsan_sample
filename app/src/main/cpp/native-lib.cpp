// Copyright (c) 2025 Bytedance Ltd. and/or its affiliates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <jni.h>
#include <pthread.h>

#include <string>

int Global;

extern "C" {
JNIEXPORT jstring JNICALL
Java_com_bytedance_tsan_1sample_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    std::string text = "Tap to run test code";
    return env->NewStringUTF(text.c_str());
}

void *Thread1(void *x) {
    Global = 42;
    return x;
}

JNIEXPORT void JNICALL
Java_com_bytedance_tsan_1sample_MainActivity_runTestCode(JNIEnv *env, jobject thiz) {
    pthread_t t;
    pthread_create(&t, nullptr, Thread1, nullptr);
    Global = 43;
    pthread_join(t, nullptr);
}

}  // extern "C"
