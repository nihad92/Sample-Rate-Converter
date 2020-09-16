package com.github.nihad.sampleconverter;

public class Utils {
  public static short[] convertBytesToAmplitudes(byte[] input, int bitsPerSample) {
    int bytePerSample = bitsPerSample / 8;
    int numSamples = input.length / bytePerSample;
    short[] amplitudes = new short[numSamples]; // 16 bit, use a short to store

    int pointer = 0;
    for (int i = 0; i < numSamples; i++) {
      short amplitude = 0;
      for (int byteNumber = 0; byteNumber < bytePerSample; byteNumber++) {
        // little endian
        amplitude |= (short) ((input[pointer++] & 0xFF) << (byteNumber * 8));
      }
      amplitudes[i] = amplitude;
    }
    return amplitudes;
  }

  public static byte[] convertAmplitudesToBytes(short[] targetSample, int bitsPerSample) {
    int bytePerSample = bitsPerSample / 8;
    int targetLength = targetSample.length * 2;
    byte[] output = new byte[targetLength];

    // convert the amplitude to bytes
    if (bytePerSample == 1) {
      for (int i = 0; i < targetLength; i++) {
        output[i] = (byte) targetSample[i];
      }
    } else {
      // suppose bytePerSample==2
      for (int i = 0; i < targetLength && i * 2 < output.length; i++) {
        // little endian
        output[i * 2] = (byte) (targetSample[i] & 0xff);
        output[i * 2 + 1] = (byte) ((targetSample[i] >> 8) & 0xff);
      }
    }
    return output;
  }
}
