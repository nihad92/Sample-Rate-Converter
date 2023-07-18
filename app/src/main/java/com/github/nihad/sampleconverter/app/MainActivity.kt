package com.github.nihad.sampleconverter.app

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.github.nihad.sampleconverter.R
import com.github.nihad.sampleconverter.SampleConverter

class MainActivity : AppCompatActivity() {
  override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    setContentView(R.layout.activity_main)
    val sampleConverter = SampleConverter(48000, 44100, 2, 32)
    sampleConverter.resample(ByteArray(4), 48000, 44100, 2, 16)
    sampleConverter.close()
  }
}