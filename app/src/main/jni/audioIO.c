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
}

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

    while(currentSamps < RECORDBUFFERSIZE){
        samps = android_AudioIn(pointer,inBuffer,VECSAMPS_MONO);
        for(i = 0; i < samps; ++i){
            memcpy(&(totalBuffer[currentSamps]), inBuffer, sizeof(inBuffer));
        }
        currentSamps += samps;
    }
    qsort(totalBuffer, RECORDBUFFERSIZE, sizeof(float), compare);
    for(i = 2 * num; i < 6 * num; ++i){
        sum += CONV16BIT * totalBuffer[i];
    }
    average = sum / (4 * num);

    return average;
}

void Java_chosanghoon_cheapylcmeter_MainActivity_genTone(JNIEnv* env, jclass clazz, jint frequency, jint millibel){
    int freq = frequency;
    int i, j;
    float sine[69][VECSAMPS_MONO];

    for(i = 0; i < 69; ++i){
        for(j = 0; j < VECSAMPS_MONO; ++j){
            sine[i][j] = sinf(2 * M_PI * (VECSAMPS_MONO * i + j) / ((double)SR / freq));
        }
    }
    for(i = 0; i < 69; ++i){
        android_AudioOut(pointer,sine[i],VECSAMPS_MONO,millibel);
    }
}


