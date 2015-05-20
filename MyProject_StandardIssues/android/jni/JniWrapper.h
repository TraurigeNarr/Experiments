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

    struct JniString //: boost::noncopyable
        {
        JNIEnv* m_env;
        jstring m_jstr;

        JniString(JNIEnv* env, const char* arg)
            : m_env(env)
            {
            //m_jstr = env->NewStringUTF(arg);
            }

        ~JniString()
            {
            //if (m_jstr)
             //   m_env->DeleteLocalRef(m_jstr);
            }

        jobject get() { return m_jstr; }
        };

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

    struct JniObject //: boost::noncopyable
        {
        JniObject(JNIEnv*, jobject){ }
        jobject get() { return jobject(); }
        };

    /////////////////////////////////////////////////////////

    struct JniHolder
        {
        jvalue val;
        JniString jString;
        JniObject jObject;

        // bool
        explicit JniHolder(JNIEnv *env, bool arg)
            : jString(env, nullptr)
            , jObject(env, jobject())
            {
            val.z = arg;
            }

        // byte
        explicit JniHolder(JNIEnv *env, unsigned char arg)
            : jString(env, nullptr)
            , jObject(env, jobject())
            {
            val.b = arg;
            }

        // char
        explicit JniHolder(JNIEnv *env, char arg)
            : jString(env, nullptr)
            , jObject(env, jobject())
            {
            val.c = arg;
            }

        // short
        explicit JniHolder(JNIEnv *env, short arg)
            : jString(env, nullptr)
            , jObject(env, jobject())
            {
            val.s = arg;
            }

        // int
        explicit JniHolder(JNIEnv *env, int arg)
            : jString(env, nullptr)
            , jObject(env, jobject())
            {
            val.i = arg;
            }

        // long
        explicit JniHolder(JNIEnv *env, long arg)
            : jString(env, nullptr)
            , jObject(env, jobject())
            {
            val.j = arg;
            }

        // float
        explicit JniHolder(JNIEnv *env, float arg)
            : jString(env, nullptr)
            , jObject(env, jobject())
            {
            val.f = arg;
            }

        // double
        explicit JniHolder(JNIEnv *env, double arg)
            : jString(env, nullptr)
            , jObject(env, jobject())
            {
            val.d = arg;
            }

        // string
        explicit JniHolder(JNIEnv *env, const char* arg)
            : jString(env, arg)
            , jObject(env, jobject())
            {
            val.l = jString.get();
            }

        // object
        explicit JniHolder(JNIEnv *env, jobject arg)
            : jString(env, nullptr)
            , jObject(env, arg)
            {
            val.l = jObject.get();
            }

        ////////////////////////////////////////////////////////

        operator jvalue()
            {
            return val;
            }

        jvalue get() { return val; }
        };

    namespace utils
        {

        //////////////////////////////////////////////
        template <typename T>
        std::string GetTypeName();

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
        std::string GetTypeName<jobject>()
            {
            return "Ljava/lang/Object;";
            }

        //////////////////////////////////////////////

        void GetType(std::string&)
            {   }

        template <typename T, typename... Args>
        void GetType(std::string& signatureString, T value, Args... args)
            {
            signatureString += GetTypeName<T>();
            GetType(signatureString, args...);
            }

        ///////////////////////////////////////////////////////////

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

        template <>
        struct Impl <std::string>
            {
            template <typename... Args>
            static std::string CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
                {
                const int size = sizeof...(args);
                jvalue vals[size] = { static_cast<jvalue>(JniHolder(env, args))... };
                return "";// env->CallStaticMethodA<std::string>(clazz, method, vals);
                }
            };

        template <typename MethodType, typename... Args>
        MethodType CallStaticMethod(const char* className, const char* mname, Args... args);

        template <typename MethodType, typename... Args>
        MethodType CallStaticMethod(const char* className, const char* mname, Args... args)
            {
            std::string signature_string = "(";
            GetType(signature_string, args...);
            signature_string += ")";

            signature_string += GetTypeName<MethodType>();

            JNIEnv *env = getEnv();
            JniClass clazz(env, className);
            jmethodID method = env->GetStaticMethodID(clazz.get(), mname, signature_string.c_str());
            return Impl<MethodType>::CallStaticMethod(env, clazz.get(), method, args...);
            }

        } // utils

    } // Android

#endif