//
// Created by Nihad Ahmetovic on 16/09/2020.
//

#include "SampleConverter.h"

extern "C" {
MultiChannelResampler *multiChannelResampler = nullptr;
JNIEXPORT void JNICALL
Java_com_github_nihad_sampleconverter_SampleConverter_clean(JNIEnv *env, jobject obj) {
    if (multiChannelResampler != nullptr) {
        delete multiChannelResampler;
        multiChannelResampler = nullptr;
    }
}

JNIEXPORT jshortArray JNICALL
Java_com_github_nihad_sampleconverter_SampleConverter_resample(JNIEnv *env, jobject obj,
                                                               jshortArray input,
                                                               jint numberOfSamples,
                                                               jint inputSampleRate,
                                                               jint outputSampleRate,
                                                               jint channelCount) {
    if (multiChannelResampler == nullptr) {
        multiChannelResampler = MultiChannelResampler::Builder()
                .setChannelCount(channelCount)
                ->setInputRate(inputSampleRate)
                ->setOutputRate(outputSampleRate)
                ->setNumTaps(32)
                ->build();
    }

    auto *inputShort = env->GetShortArrayElements(input, JNI_FALSE);
    auto *inputBuffer = new float[numberOfSamples];
    convertPcm16ToFloat(inputShort, inputBuffer, numberOfSamples);

    int numOutputSamples =
            (int) (((float) outputSampleRate / (float) inputSampleRate) * (float) numberOfSamples) +
            1;
    auto *outputBuffer = new float[numOutputSamples];
    auto *outputBufferPointer = outputBuffer;
    int numOutputFrames = 0;


    int inputFramesLeft = numberOfSamples / channelCount;
    while (inputFramesLeft > 0) {
        if (multiChannelResampler->isWriteNeeded()) {
            multiChannelResampler->writeNextFrame(inputBuffer);
            inputBuffer += channelCount;
            inputFramesLeft--;
        } else {
            multiChannelResampler->readNextFrame(outputBufferPointer);
            outputBufferPointer += channelCount;
            numOutputFrames++;
        }
    }

    int outputSize = numOutputFrames * channelCount;
    auto *output = new short[outputSize];
    convertFloatToPcm16(outputBuffer, output, outputSize);
    jshortArray result = env->NewShortArray(outputSize);
    env->SetShortArrayRegion(result, 0, outputSize, output);
    return result;
}

}
