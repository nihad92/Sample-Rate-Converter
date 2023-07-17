//
// Created by Nihad Ahmetovic on 17. 7. 2023..
//
#include "common/handle.h"
#include "resampler/MultiChannelResampler.h"
#include <jni.h>
#include <oboe/Oboe.h>

using namespace resampler;
using namespace oboe;

jobject g_object;

extern "C"
JNIEXPORT jshortArray
JNICALL Java_com_github_nihad_sampleconverter_SampleConverter_resample(JNIEnv *env, jobject obj,
                                                                       jshortArray input,
                                                                       jint numberOfSamples,
                                                                       jint inputSampleRate,
                                                                       jint outputSampleRate,
                                                                       jint channelCount
) {
    auto *resampler = getHandle<MultiChannelResampler>(env, obj);
    auto *inputShort = env->GetShortArrayElements(input, JNI_FALSE);
    auto *inputBuffer = new float[numberOfSamples];
    convertPcm16ToFloat(inputShort, inputBuffer, numberOfSamples
    );

    int numOutputSamples =
            (int) (((float) outputSampleRate / (float) inputSampleRate) * (float) numberOfSamples) +
            1;
    auto *outputBuffer = new float[numOutputSamples];
    auto *outputBufferPointer = outputBuffer;
    int numOutputFrames = 0;


    int inputFramesLeft = numberOfSamples / channelCount;
    while (inputFramesLeft > 0) {
        if (resampler->isWriteNeeded()) {
            resampler->writeNextFrame(inputBuffer);
            inputBuffer +=
                    channelCount;
            inputFramesLeft--;
        } else {
            resampler->readNextFrame(outputBufferPointer);
            outputBufferPointer += channelCount;
            numOutputFrames++;
        }
    }

    int outputSize = numOutputFrames * channelCount;
    auto *output = new short[outputSize];
    convertFloatToPcm16(outputBuffer, output, outputSize);
    jshortArray result = env->NewShortArray(outputSize);
    env->SetShortArrayRegion(result,0, outputSize, output);
    return result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_github_nihad_sampleconverter_SampleConverter_initialize(JNIEnv *env, jobject thiz,
                                                                 jint input_sample_rate,
                                                                 jint output_sample_rate,
                                                                 jint channel_count) {
    g_object = createGlobalReference(env, thiz, thiz);
    auto *resampler = MultiChannelResampler::Builder()
            .setChannelCount(channel_count)
            ->setInputRate(input_sample_rate)
            ->setOutputRate(output_sample_rate)
            ->setNumTaps(32)
            ->build();

    setHandle(env, g_object, resampler);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_github_nihad_sampleconverter_SampleConverter_close(JNIEnv *env, jobject thiz) {
    auto *resampler = getHandle<MultiChannelResampler>(env, thiz);
    clearGlobalReferences(env, thiz);
    delete resampler;
}