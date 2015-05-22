#ifndef __JniWrapper_h__
#define __JniWrapper_h__

#include <jni.h>
#include <string>
#include <exception>

//#include <boost/noncopyable.hpp>

namespace Android
    {
	static JavaVM *javaVM = NULL;

	JNIEnv* getEnv()
		{
		JNIEnv *env = NULL;
		int status = javaVM->AttachCurrentThread(&env, NULL);
		if (status < 0) {
			return NULL;
			}

		return env;
		}

    class JniClass //: private boost::noncopyable
        {
        JNIEnv* m_env;
        jclass m_class;
        public:
            JniClass(JNIEnv* env, const char* name)
                : m_env(env)
                {
                m_class = m_env->FindClass(name);
                }

            ~JniClass()
                {
                if (m_class) 
                    m_env->DeleteLocalRef(m_class);
                }

            jclass get() { return m_class; }
        };

	/////////////////////////////////////////////////////////

	struct JObjectHolder
		{
		jobject jObject;
		JNIEnv* m_env;

		JObjectHolder()
			: m_env(nullptr)
			{}

		JObjectHolder(JNIEnv* env, jobject obj)
			: jObject(obj)
			, m_env(env)
			{}

		~JObjectHolder()
			{
			if (jObject && m_env != nullptr)
				m_env->DeleteLocalRef(jObject);
			}

		jobject get() { return jObject; }
		};

    /////////////////////////////////////////////////////////

    struct JniHolder
        {
        jvalue val;
		JObjectHolder jObject;

        // bool
        explicit JniHolder(JNIEnv *env, bool arg)
            : jObject(env, jobject())
            {
            val.z = arg;
            }

        // byte
        explicit JniHolder(JNIEnv *env, unsigned char arg)
            : jObject(env, jobject())
            {
            val.b = arg;
            }

        // char
        explicit JniHolder(JNIEnv *env, char arg)
            : jObject(env, jobject())
            {
            val.c = arg;
            }

        // short
        explicit JniHolder(JNIEnv *env, short arg)
            : jObject(env, jobject())
            {
            val.s = arg;
            }

        // int
        explicit JniHolder(JNIEnv *env, int arg)
            : jObject(env, jobject())
            {
            val.i = arg;
            }

        // long
        explicit JniHolder(JNIEnv *env, long arg)
            : jObject(env, jobject())
            {
            val.j = arg;
            }

        // float
        explicit JniHolder(JNIEnv *env, float arg)
            : jObject(env, jobject())
            {
            val.f = arg;
            }

        // double
        explicit JniHolder(JNIEnv *env, double arg)
            : jObject(env, jobject())
            {
            val.d = arg;
            }

        // string
        explicit JniHolder(JNIEnv *env, const char* arg)
			: jObject(env, env->NewStringUTF(arg))
            {
            val.l = jObject.get();
            }

        // object
        explicit JniHolder(JNIEnv *env, jobject arg)
            : jObject(env, arg)
            {
            val.l = jObject.get();
            }

        ////////////////////////////////////////////////////////

		operator jvalue() { return val; }

        jvalue get() { return val; }
        };


	namespace impl
	{

	template <typename T>
	std::string GetTypeName();
	/////////////////////////////////////////////////////////
	// GetTypeName partial specializations

	template <>
	std::string GetTypeName<void>()
		{
		return "V";
		}

	template <>
	std::string GetTypeName<bool>()
		{
		return "Z";
		}

	template <>
	std::string GetTypeName<unsigned char>()
		{
		return "B";
		}

	template <>
	std::string GetTypeName<char>()
		{
		return "C";
		}

	template <>
	std::string GetTypeName<short>()
		{
		return "S";
		}

	template <>
	std::string GetTypeName<int>()
		{
		return "I";
		}

	template <>
	std::string GetTypeName<long>()
		{
		return "J";
		}

	template <>
	std::string GetTypeName<float>()
		{
		return "F";
		}

	template <>
	std::string GetTypeName<double>()
		{
		return "D";
		}

	template <>
	std::string GetTypeName<const char*>()
		{
		return "Ljava/lang/String;";
		}

	template <>
	std::string GetTypeName<std::string>()
		{
		return "Ljava/lang/String;";
		}

	template <>
	std::string GetTypeName<jobject>()
		{
		return "Ljava/lang/Object;";
		}

	/////////////////////////////////////////////////////////
	// GetType

	template <typename T>
	std::string GetType(const T&)
		{
		return GetTypeName<T>();
		}

	/////////////////////////////////////////////////////////
	// Recursive GetType

	void GetTypeRecursive(std::string&)
		{   }

	template <typename T, typename... Args>
	void GetTypeRecursive(std::string& signatureString, T value, Args... args)
		{
		signatureString += GetTypeName<T>();
		GetTypeRecursive(signatureString, args...);
		}

	/////////////////////////////////////////////////////////
	// Impl class interface -> call all needed methods; 
	//		Realizations in partial specializations

	template <typename MethodType>
	struct Impl
		{
		// for each type need concrete method in JNI
		//  int -> CallIntMethod
		//  void -> CallVoidMethod...
		template <typename... Args>
		static MethodType CallMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args);

		template <typename... Args>
		static void CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args);
		};

	/////////////////////////////////////////////////////////
	// Impl<void>

	template <>
	struct Impl <void>
		{
		template <typename... Args>
		static void CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
			{
			const int size = sizeof...(args);
			if (size != 0)
				{
				jvalue vals[size] = { static_cast<jvalue>(JniHolder(env, args))... };
				return env->CallStaticVoidMethodA(clazz, method, vals);
				}

			return env->CallStaticVoidMethod(clazz, method);
			}
		};

	/////////////////////////////////////////////////////////
	// Impl<int>

	template <>
	struct Impl <int>
		{
		template <typename... Args>
		static int CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
			{
			const int size = sizeof...(args);
			if (size != 0)
				{
				jvalue vals[size] = { static_cast<jvalue>(JniHolder(env, args))... };
				return env->CallStaticIntMethodA(clazz, method, vals);
				}

			return env->CallStaticIntMethod(clazz, method);
			}
		};

	/////////////////////////////////////////////////////////
	// Impl<float>

	template <>
	struct Impl <float>
		{
		template <typename... Args>
		static float CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
			{
			const int size = sizeof...(args);
			if (size != 0)
				{
				jvalue vals[size] = { static_cast<jvalue>(JniHolder(env, args))... };
				return env->CallStaticFloatMethodA(clazz, method, vals);
				}

			return env->CallStaticFloatMethod(clazz, method);
			}
		};

	/////////////////////////////////////////////////////////
	// Impl<std::string>

	template <>
	struct Impl <std::string>
		{
		template <typename... Args>
		static std::string CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
			{
			const int size = sizeof...(args);
			jstring jstr;
			if (size != 0)
				{
				jvalue vals[size] = { static_cast<jvalue>(JniHolder(env, args))... };
				jstr = (jstring)env->CallStaticObjectMethodA(clazz, method, vals);
				}
			else
				jstr = (jstring)env->CallStaticObjectMethod(clazz, method);

			if (!jstr)
				return std::string();

			const char* chars = env->GetStringUTFChars(jstr, NULL);
			std::string result;
			if (chars)
				result = chars;

			env->ReleaseStringUTFChars(jstr, chars);
			return result;
			}
		};

	} // impl

    
	/////////////////////////////////////////////////////////
	// CallStaticMethod implemetation

	template <typename MethodType, typename... Args>
	MethodType CallStaticMethod(const char* className, const char* mname, Args... args)
		{
		using namespace impl;
		const size_t arg_num = sizeof...(Args);
		std::string signatures[arg_num] = { GetType(args)... };

		std::string signature_string;
		signature_string.reserve(15);
		signature_string += "(";
		for (size_t i = 0; i < arg_num; ++i)
			signature_string += signatures[i];
		signature_string += ")";
		signature_string += GetTypeName<MethodType>();

		JNIEnv *env = getEnv();
		JniClass clazz(env, className);
		jmethodID method = env->GetStaticMethodID(clazz.get(), mname, signature_string.c_str());
		return Impl<MethodType>::CallStaticMethod(env, clazz.get(), method, args...);
		}

	/////////////////////////////////////////////////////////
	// CallMethod implemetation

    } // Android

#endif