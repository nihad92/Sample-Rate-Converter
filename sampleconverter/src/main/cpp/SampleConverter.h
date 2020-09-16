//
// Created by Nihad Ahmetovic on 16/09/2020.
//

#ifndef SAMPLE_CONVERTER_SAMPLECONVERTER_H
#define SAMPLE_CONVERTER_SAMPLECONVERTER_H

#include <jni.h>
#include <SampleRateConverter.h>
#include <oboe/Oboe.h>

using namespace oboe::flowgraph;
using namespace resampler;
using namespace oboe;

extern "C" {
JNIEXPORT void JNICALL
Java_com_github_nihad_sampleconverter_SampleConverter_clean(JNIEnv *env, jobject obj);

JNIEXPORT jshortArray JNICALL
Java_com_github_nihad_sampleconverter_SampleConverter_resample(JNIEnv *env, jobject obj,
                                                               jshortArray input,
                                                               jint numberOfSamples,
                                                               jint inputSampleRate,
                                                               jint outputSampleRate,
                                                               jint channelCount);
}


#endif //SAMPLE_CONVERTER_SAMPLECONVERTER_H
