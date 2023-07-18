package com.github.nihad.sampleconverter

import com.github.nihad.sampleconverter.common.IHandle

class SampleConverter : IHandle, AutoCloseable {
    init {
        System.loadLibrary("oboe-sample-converter")
    }

    constructor(inputSampleRate: Int, outputSampleRate: Int, channelCount: Int, numOfTaps: Int = 32) {
        initialize(inputSampleRate, outputSampleRate, channelCount, numOfTaps)
    }

    private external fun initialize(inputSampleRate: Int, outputSampleRate: Int, channelCount: Int, numOfTaps: Int)

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

    external override fun close();

    override val nativeHandle: Long = 0

}