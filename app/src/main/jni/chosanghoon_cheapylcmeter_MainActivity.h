/*
  chosanghoon_cheapylcmeter_MainActivity.h
  Header for class chosanghoon_cheapylcmeter_MainActivity
  Copyright (c) 2016, Sanghoon Cho
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
  * Neither the name of the <organization> nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <jni.h>

#ifndef _Included_chosanghoon_cheapylcmeter_MainActivity
#define _Included_chosanghoon_cheapylcmeter_MainActivity
#ifdef __cplusplus
extern "C" {
#endif
#undef chosanghoon_cheapylcmeter_MainActivity_BIND_ABOVE_CLIENT
#define chosanghoon_cheapylcmeter_MainActivity_BIND_ABOVE_CLIENT 8L
#undef chosanghoon_cheapylcmeter_MainActivity_BIND_ADJUST_WITH_ACTIVITY
#define chosanghoon_cheapylcmeter_MainActivity_BIND_ADJUST_WITH_ACTIVITY 128L
#undef chosanghoon_cheapylcmeter_MainActivity_BIND_ALLOW_OOM_MANAGEMENT
#define chosanghoon_cheapylcmeter_MainActivity_BIND_ALLOW_OOM_MANAGEMENT 16L
#undef chosanghoon_cheapylcmeter_MainActivity_BIND_AUTO_CREATE
#define chosanghoon_cheapylcmeter_MainActivity_BIND_AUTO_CREATE 1L
#undef chosanghoon_cheapylcmeter_MainActivity_BIND_DEBUG_UNBIND
#define chosanghoon_cheapylcmeter_MainActivity_BIND_DEBUG_UNBIND 2L
#undef chosanghoon_cheapylcmeter_MainActivity_BIND_IMPORTANT
#define chosanghoon_cheapylcmeter_MainActivity_BIND_IMPORTANT 64L
#undef chosanghoon_cheapylcmeter_MainActivity_BIND_NOT_FOREGROUND
#define chosanghoon_cheapylcmeter_MainActivity_BIND_NOT_FOREGROUND 4L
#undef chosanghoon_cheapylcmeter_MainActivity_BIND_WAIVE_PRIORITY
#define chosanghoon_cheapylcmeter_MainActivity_BIND_WAIVE_PRIORITY 32L
#undef chosanghoon_cheapylcmeter_MainActivity_CONTEXT_IGNORE_SECURITY
#define chosanghoon_cheapylcmeter_MainActivity_CONTEXT_IGNORE_SECURITY 2L
#undef chosanghoon_cheapylcmeter_MainActivity_CONTEXT_INCLUDE_CODE
#define chosanghoon_cheapylcmeter_MainActivity_CONTEXT_INCLUDE_CODE 1L
#undef chosanghoon_cheapylcmeter_MainActivity_CONTEXT_RESTRICTED
#define chosanghoon_cheapylcmeter_MainActivity_CONTEXT_RESTRICTED 4L
#undef chosanghoon_cheapylcmeter_MainActivity_MODE_APPEND
#define chosanghoon_cheapylcmeter_MainActivity_MODE_APPEND 32768L
#undef chosanghoon_cheapylcmeter_MainActivity_MODE_ENABLE_WRITE_AHEAD_LOGGING
#define chosanghoon_cheapylcmeter_MainActivity_MODE_ENABLE_WRITE_AHEAD_LOGGING 8L
#undef chosanghoon_cheapylcmeter_MainActivity_MODE_MULTI_PROCESS
#define chosanghoon_cheapylcmeter_MainActivity_MODE_MULTI_PROCESS 4L
#undef chosanghoon_cheapylcmeter_MainActivity_MODE_PRIVATE
#define chosanghoon_cheapylcmeter_MainActivity_MODE_PRIVATE 0L
#undef chosanghoon_cheapylcmeter_MainActivity_MODE_WORLD_READABLE
#define chosanghoon_cheapylcmeter_MainActivity_MODE_WORLD_READABLE 1L
#undef chosanghoon_cheapylcmeter_MainActivity_MODE_WORLD_WRITEABLE
#define chosanghoon_cheapylcmeter_MainActivity_MODE_WORLD_WRITEABLE 2L
#undef chosanghoon_cheapylcmeter_MainActivity_DEFAULT_KEYS_DIALER
#define chosanghoon_cheapylcmeter_MainActivity_DEFAULT_KEYS_DIALER 1L
#undef chosanghoon_cheapylcmeter_MainActivity_DEFAULT_KEYS_DISABLE
#define chosanghoon_cheapylcmeter_MainActivity_DEFAULT_KEYS_DISABLE 0L
#undef chosanghoon_cheapylcmeter_MainActivity_DEFAULT_KEYS_SEARCH_GLOBAL
#define chosanghoon_cheapylcmeter_MainActivity_DEFAULT_KEYS_SEARCH_GLOBAL 4L
#undef chosanghoon_cheapylcmeter_MainActivity_DEFAULT_KEYS_SEARCH_LOCAL
#define chosanghoon_cheapylcmeter_MainActivity_DEFAULT_KEYS_SEARCH_LOCAL 3L
#undef chosanghoon_cheapylcmeter_MainActivity_DEFAULT_KEYS_SHORTCUT
#define chosanghoon_cheapylcmeter_MainActivity_DEFAULT_KEYS_SHORTCUT 2L
#undef chosanghoon_cheapylcmeter_MainActivity_RESULT_CANCELED
#define chosanghoon_cheapylcmeter_MainActivity_RESULT_CANCELED 0L
#undef chosanghoon_cheapylcmeter_MainActivity_RESULT_FIRST_USER
#define chosanghoon_cheapylcmeter_MainActivity_RESULT_FIRST_USER 1L
#undef chosanghoon_cheapylcmeter_MainActivity_RESULT_OK
#define chosanghoon_cheapylcmeter_MainActivity_RESULT_OK -1L
/*
 * Class:     chosanghoon_cheapylcmeter_MainActivity
 * Method:    openAudioDevice
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_chosanghoon_cheapylcmeter_MainActivity_openAudioDevice
  (JNIEnv *, jclass);

/*
 * Class:     chosanghoon_cheapylcmeter_MainActivity
 * Method:    closeAudioDevice
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_chosanghoon_cheapylcmeter_MainActivity_closeAudioDevice
  (JNIEnv *, jclass);

/*
 * Class:     chosanghoon_cheapylcmeter_MainActivity
 * Method:    getAmplitude
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_chosanghoon_cheapylcmeter_MainActivity_getAmplitude
  (JNIEnv *, jclass, jint);

/*
 * Class:     chosanghoon_cheapylcmeter_MainActivity
 * Method:    genTone
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_chosanghoon_cheapylcmeter_MainActivity_genTone
  (JNIEnv *, jclass, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
