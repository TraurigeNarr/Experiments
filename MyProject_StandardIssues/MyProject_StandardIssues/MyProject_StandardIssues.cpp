// MyProject_StandardIssues.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <string>
#include <vector>
#include <memory>

#include <boost/noncopyable.hpp>

/////////

struct jobject {    };
struct jclass { };
typedef int jmethodID;

typedef union jvalue {
    bool z;
    unsigned char b;
    char    c;
    short   s;
    int     i;
    long    j;
    float   f;
    double  d;
    jobject l;
    } jvalue;

struct JNIEnv
    {
    //void CallStaticMethodA(jclass jniClass, jmethodID jniMethod, jvalue*){}
    template <typename T>
    T CallStaticMethodA(jclass jniClass, jmethodID jniMethod, ...){ return T(); }
    };

struct JniString
    {
    JniString(JNIEnv*, const char*){}
    jobject get() { return jobject(); }
    };

struct JniObject : boost::noncopyable
    {
    JniObject(JNIEnv*, jobject){}
    jobject get() { return jobject(); }
    };

class JniClass
    : private boost::noncopyable
    {
    public:
        jclass get() {
            return jclass();
            }
    };

/////////////////////////////
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

    //////////////////////////////////////////////

    void CallToJni(JNIEnv* env, jclass clazz, jmethodID method)
        {
        std::cout << "Wow: zero arguments\n";
        env->CallStaticMethodA<void>(clazz, method);
        }

    template <typename... Args>
    void CallToJni(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
        {
        std::cout << "Wow: " << sizeof...(args) << "\n";
        const int size = sizeof...(args);
        jvalue vals[size] = { (static_cast<jvalue>(JniHolder(env, args)), 0)... };
        env->CallStaticMethodA<void>(clazz, method, vals);
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
            std::cout << "void method\n";
            CallToJni(env, clazz, method, args...);
            return;
            }
        };

    template <>
    struct Impl <int>
        {
        template <typename... Args>
        static int CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
            {
            std::cout << "int method\n";
            const int size = sizeof...(args);
            jvalue vals[size] = { (static_cast<jvalue>(JniHolder(env, args)), 0)... };
            return env->CallStaticMethodA<int>(clazz, method, vals);
            }
        };

    template <>
    struct Impl <std::string>
        {
        template <typename... Args>
        static std::string CallStaticMethod(JNIEnv* env, jclass clazz, jmethodID method, Args... args)
            {
            std::cout << "int method\n";
            const int size = sizeof...(args);
            jvalue vals[size] = { (static_cast<jvalue>(JniHolder(env, args)), 0)... };
            return env->CallStaticMethodA<std::string>(clazz, method, vals);
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

        std::cout << "Signature: " << signature_string << std::endl;
        jclass clazz;
        jmethodID method = 0;
        JNIEnv env;
        return Impl<MethodType>::CallStaticMethod(&env, clazz, method, args...);
        }

    } // utils

/////////

int _tmain(int argc, _TCHAR* argv[])
    {
	time_t f;
    std::cout << "0 parameters\n";
    utils::CallStaticMethod<void>("a", "b");
    
    std::cout << "\n1 parameter\n";
    utils::CallStaticMethod<void>("a", "b", 1);
    std::cout << "\n2 parameters\n";
    utils::CallStaticMethod<void>("a", "b", 1, "s");
    std::cout << "\n3 parameters\n";
    int a = utils::CallStaticMethod<int>("a", "b", 1, "s", 1.3f);

    std::cout << "\n3 parameters\n";
    utils::CallStaticMethod<void>("a", "b", 1, "s", 1.3);

    std::cout << "\n6 parameters\n";
    utils::CallStaticMethod<void>("a", "b", 1, "s", 1.3, true, '5', 1.f);


    return 0;
    }

