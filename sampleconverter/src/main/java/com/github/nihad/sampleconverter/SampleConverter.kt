package com.github.nihad.sampleconverter

class SampleConverter {
    init {
        System.loadLibrary("oboe-sample-converter")
    }

    fun cleanResampler() {
        clean()
    }

    fun resample(input: ByteArray,
                 inputSampleRate: Int,
                 outputSampleRate: Int,
                 channelCount: Int,
                 bitsPerSample: Int): ByteArray {
        val amplitudes = Utils.convertBytesToAmplitudes(input, bitsPerSample);
        val targetSample = resample(amplitudes, amplitudes.size, inputSampleRate, outputSampleRate, channelCount)
        return Utils.convertAmplitudesToBytes(targetSample, bitsPerSample);
    }

    private external fun resample(inputBuffer: ShortArray,
                                  numberOfSamples: Int,
                                  inputSampleRate: Int,
                                  outputSampleRate: Int,
                                  channelCount: Int): ShortArray

    private external fun clean()
}