// Copyright (c) 2025 Bytedance Ltd. and/or its affiliates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
package com.bytedance.tsan_sample

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.bytedance.tsan_sample.databinding.ActivityMainBinding


class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.sampleText.text = stringFromJNI()

        binding.root.setOnClickListener {
            runTestCode()
        }
    }

    /**
     * A native method that is implemented by the 'tsan_sample' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun runTestCode()

    companion object {
        // Used to load the 'tsan_sample' library on application startup.
        init {
            System.loadLibrary("tsan_sample")
        }
    }
}
