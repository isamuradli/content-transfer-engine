/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* Distributed under BSD 3-Clause license.                                   *
* Copyright by The HDF Group.                                               *
* Copyright by the Illinois Institute of Technology.                        *
* All rights reserved.                                                      *
*                                                                           *
* This file is part of Hermes. The full Hermes copyright notice, including  *
* terms governing use, modification, and redistribution, is contained in    *
* the COPYING file, which can be found at the top directory. If you do not  *
* have access to the file, you may request a copy from help@hdfgroup.org.   *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "java_wrapper.h"
#include "c/c_wrapper.h"
#include <jni.h>

/** Get Hermes instance */

/** Create Hermes instance */
JNIEXPORT void JNICALL Java_com_hermes_Hermes_create(
    JNIEnv *env, jobject obj, jstring str) {
  const char* cstr = env->GetStringUTFChars(str, NULL);
  env->ReleaseStringUTFChars(str, cstr);
}