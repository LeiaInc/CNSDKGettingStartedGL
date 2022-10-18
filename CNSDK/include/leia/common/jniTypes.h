#pragma once

#include "leia/common/config.h"

#ifdef LNK_HAS_JNI
#include <jni.h>
#else
class _jobject {};
using jobject = _jobject*;
struct _JavaVM;
using JavaVM = _JavaVM;
struct _JNIEnv;
using JNIEnv = _JNIEnv;
#endif // LNK_HAS_JNI
