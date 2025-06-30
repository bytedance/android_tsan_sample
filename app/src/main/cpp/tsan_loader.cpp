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
#include <android/log.h>
#include <dlfcn.h>
#include <sys/personality.h>
#include <unistd.h>

#include <string>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, kTag, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, kTag, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, kTag, __VA_ARGS__)
#define LOGF(...) \
    __android_log_print(ANDROID_LOG_FATAL, kTag, __VA_ARGS__); \
    __builtin_trap()

const static char kTag[] = "TSAN_LOADER";
const static char kTSANLib[] = "libclang_rt.tsan-aarch64-android.so";
const static char kTSANEnvPrefix[] = "TSAN_LIB=";

std::string CurrentModulePath() {
    Dl_info info;
    dladdr((const void *) (&CurrentModulePath), &info);
    std::string path = info.dli_fname;
    return path.substr(0, path.find_last_of('/'));
}

std::string GetTSANLibPath(char *envp[]) {
    const size_t len = strlen(kTSANEnvPrefix);
    for (int i = 0; envp[i]; ++i) {
        if (strncmp(envp[i], kTSANEnvPrefix, len) == 0) {
            return { envp[i] + len };
        }
    }
    return CurrentModulePath() + "/" + kTSANLib;
}

int main(int argc, char *argv[], char *envp[]) {
    int old_personality = personality(0xffffffff);

    LOGI("personality = %x", old_personality);
    bool aslr_on = (old_personality & ADDR_NO_RANDOMIZE) == 0;
    if (aslr_on) {
        // for debug
        if (argc == 1) {
            personality(old_personality | ADDR_NO_RANDOMIZE);
            execve(argv[0], argv, envp);
            LOGF("execve failed");
        }

        size_t len = 0;
        while (envp[len]) {
            len++;
        }

        char **new_envp = static_cast<char **>(malloc((len + 2) * sizeof(char *)));
        memcpy(new_envp, envp, len * sizeof(char*));
        new_envp[len + 1] = nullptr;

        std::string preload_env = "LD_PRELOAD=" + GetTSANLibPath(envp);
        LOGI("%s", preload_env.c_str());
        new_envp[len] = const_cast<char*>(preload_env.c_str());

        personality(old_personality | ADDR_NO_RANDOMIZE);
        execve(argv[1], &argv[1], new_envp);
        LOGF("execve failed");
    } else {
        // for debug
        LOGI("==================");
        for (int i = 0; i < argc; ++i) {
            LOGI("argv[%d] = %s", i, argv[i]);
        }

        LOGI("==================");
        for (int i = 0; envp[i]; ++i) {
            LOGI("envp[%d] = %s", i, envp[i]);
        }
    }
    return 0;
}
