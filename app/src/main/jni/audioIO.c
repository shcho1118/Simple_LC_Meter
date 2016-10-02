/*
  audioIO.c:
  Tone generation/amplitude calculator module
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

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <android/log.h>
#include "opensl_io.h"
#include "chosanghoon_cheapylcmeter_MainActivity.h"

#define BUFFERFRAMES 4416
#define RECORDBUFFERSIZE 4416*5//almost 500ms
#define VECSAMPS_MONO 64
#define SR 44100
#define CONV16BIT 32767

static OPENSL_STREAM  *pointer;

int compare(const void *a, const void *b)
{
    float num1 = *(float *)a;
    float num2 = *(float *)b;

    if (num1 > num2)
        return -1;

    if (num1 < num2)
        return 1;

    return 0;
}//qsort 오름차순 내림차순 결정

void Java_chosanghoon_cheapylcmeter_MainActivity_openAudioDevice(JNIEnv* env, jclass clazz){
    pointer = android_OpenAudioDevice(SR,1,1,BUFFERFRAMES);
    assert(pointer != NULL);
}

void Java_chosanghoon_cheapylcmeter_MainActivity_closeAudioDevice(JNIEnv* env, jclass clazz){
    android_CloseAudioDevice(pointer);
}

jfloat Java_chosanghoon_cheapylcmeter_MainActivity_getAmplitude(JNIEnv* env, jclass clazz, jint frequency){
    int i;
    int samps;
    int currentSamps = 0;
    int num = ((float)frequency * ((float)BUFFERFRAMES/(float)SR))/8;
    float sum = 0;
    float average = 0;
    float inBuffer[VECSAMPS_MONO];
    float totalBuffer[RECORDBUFFERSIZE];
    memset(pointer->inputBuffer[0], 0, sizeof(pointer->inputBuffer[0]));
    memset(pointer->inputBuffer[1], 0, sizeof(pointer->inputBuffer[1]));
    //inputbuffer 비우기
    while(currentSamps < RECORDBUFFERSIZE){
        samps = android_AudioIn(pointer,inBuffer,VECSAMPS_MONO);
        for(i = 0; i < samps; ++i){
            memcpy(&(totalBuffer[currentSamps]), inBuffer, sizeof(inBuffer));
        }
        currentSamps += samps;
    }//오디오 데이터 약 500ms동안 저장
    qsort(totalBuffer, RECORDBUFFERSIZE, sizeof(float), compare);
    for(i = 2 * num; i < 6 * num; ++i){
        sum += CONV16BIT * totalBuffer[i];
    }
    average = sum / (4 * num);
    //qsort로 sine wave의 peak들을 구한다음 일부분의 평균계산
    return average;
}

void Java_chosanghoon_cheapylcmeter_MainActivity_genTone(JNIEnv* env, jclass clazz, jint frequency, jint millibel){
    int freq = frequency;
    int i, j;
    float sine[69][VECSAMPS_MONO];//69 * 64 / 44100 is almost 100ms
    memset(pointer->outputBuffer[0], 0, sizeof(pointer->outputBuffer[0]));
    memset(pointer->outputBuffer[1], 0, sizeof(pointer->outputBuffer[1]));

    for(i = 0; i < 69; ++i){
        for(j = 0; j < VECSAMPS_MONO; ++j){
            sine[i][j] = sinf(2 * M_PI * (VECSAMPS_MONO * i + j) / ((double)SR / freq));
        }//sine wave생성
        android_AudioOut(pointer,sine[i],VECSAMPS_MONO,millibel);//sine wave 재생
    }
}


