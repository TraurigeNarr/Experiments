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
 */
package com.example.hellojni;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;


public class HelloJni extends Activity
{
	public static String mOutString = "";

	public native String  nativeTestSpeed();
	public native String  nativeTestMethods();

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

		nativeTestMethods();
		nativeTestSpeed();

        TextView tv = new TextView(this);
        tv.setText( mOutString );
        setContentView(tv);
    }

	public static void SetTime(long i, long j)
	{
		mOutString += "usual: " + i + ";\n wrapper: " + j + "\n";
		mOutString += "j/i=" + (float)j/i;
	}

	public static void TestMethod(int i)
	{
	}

	public static String TestMethod(String par)
	{
		mOutString += "return String: " + par + "\n";
		return par;
	}

	public static float TestMethod(String par, float x)
	{
		mOutString += "float String: " + par + " float=" + x  + "\n";
		return x;
	}

	public static void TestMethod(float x)
	{
		mOutString += "void float=" + x  + "\n";
	}

	public static void Test()
	{
		mOutString += " void 0\n";
	}

	public static void Test(int i)
	{
		mOutString += " void 1: Integer " + i + "\n";
	}

	public static int Test(int i, int j)
	{
		mOutString += "int 2; integer " + i + " : " + j + "\n";
		return i + j;
	}

    public native String  stringFromJNI();
    static {
        System.loadLibrary("hello-jni");
    }
}
