/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string>
#include <jni.h>

#include "JniWrapper.h"

jint JNI_OnLoad(JavaVM *vm, void *reserved) 
{
	Android::javaVM = vm;
	return JNI_VERSION_1_4;
}


extern "C" jstring Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    return env->NewStringUTF("Hello from JNI !  Compiled with ABI");
}

extern "C" void Java_com_example_hellojni_HelloJni_nativeTestSpeed(JNIEnv* env, jobject thiz)
	{
	const std::string className = "com/example/hellojni/HelloJni";

	const int COUNT = 10000;

	time_t t_begin = clock();

	for (int i = 0; i < COUNT; ++i)
		{
		JNIEnv* _env = Android::getEnv();
		Android::JniClass clazz(_env, className.c_str());
		jmethodID method = _env->GetStaticMethodID(clazz.get(), "TestMethod", "(I)V");
		env->CallStaticVoidMethod(clazz.get(), method, 1);
		}

	long t_usual_end = clock() - t_begin;
	
	t_begin = clock();
	for (int i = 0; i < COUNT; ++i)
		{
		Android::CallStaticMethod<void>(className.c_str(), "TestMethod", 1);
		}

	long t_wrapper_end = clock() - t_begin;
	
	Android::CallStaticMethod<void>(className.c_str(), "SetTime", t_usual_end, t_wrapper_end);

	}


extern "C" void Java_com_example_hellojni_HelloJni_nativeTestMethods(JNIEnv* env, jobject thiz)
    {
    const std::string className = "com/example/hellojni/HelloJni";

	std::string sRes = Android::CallStaticMethod<std::string>(className.c_str(), "TestMethod", "some");
	float fRes = Android::CallStaticMethod<float>(className.c_str(), "TestMethod", sRes.c_str(), 4.2f);
	Android::CallStaticMethod<void>(className.c_str(), "TestMethod", fRes);
    }