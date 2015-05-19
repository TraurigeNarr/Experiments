// MyProject_StandardIssues.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <string>
#include <vector>
#include <memory>

#include <boost/noncopyable.hpp>

/////////

struct jobject
    {

    };

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
    void CallStaticMethodA(int jniClass, int jniMethod, jvalue*){}
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

void Convert(JNIEnv* jEnv, std::vector<JniHolder>& objects)
    {
    }

template <typename T, typename... Args>
void Convert(JNIEnv* jEnv, std::vector<JniHolder>& objects, T value, Args... args)
    {
    objects.push_back(JniHolder(value));
    Convert(jEnv, objects, args...);
    }

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

    void CallRealJni(JNIEnv* env, int jniClass, int jniMethod, jvalue* valArray)
        {
        env->CallStaticMethodA(jniClass, jniMethod, valArray);
        }

    void CallToJni()
        {
        std::cout << "0\n";
        //JNIEnv env;
        //CallRealJni(&env, 0, 1, nullptr);
        }

    template <typename T>
    void CallToJni(T t)
        {
        std::cout << "1\n";
        JNIEnv env;
        jvalue vals[1];
        vals[0] = static_cast<jvalue>(JniHolder{ &env, t });
        CallRealJni(&env, 0, 1, vals);
        }

    template <typename T, typename T1>
    void CallToJni(T t, T1 t1)
        {
        std::cout << "2\n";
        JNIEnv env;
        jvalue vals[2];
        vals[0] = static_cast<jvalue>(JniHolder{ &env, t });
        vals[1] = static_cast<jvalue>(JniHolder{ &env, t1 });
        CallRealJni(&env, 0, 1, vals);
        }

    template <typename T, typename T1, typename T2>
    void CallToJni(T t, T1 t1, T2 t2)
        {
        std::cout << "3\n";
        JNIEnv env;
        jvalue vals[3];
        vals[0] = static_cast<jvalue>(JniHolder{ &env, t });
        vals[1] = static_cast<jvalue>(JniHolder{ &env, t1 });
        vals[2] = static_cast<jvalue>(JniHolder{ &env, t2 });
        CallRealJni(&env, 0, 1, vals);
        }

    template <typename T, typename T1, typename T2, typename T3>
    void CallToJni(T t, T1 t1, T2 t2, T3 t3)
        {
        std::cout << "4\n";
        JNIEnv env;
        jvalue vals[4];
        vals[0] = static_cast<jvalue>(JniHolder(&env, t));
        vals[1] = static_cast<jvalue>(JniHolder(&env, t1));
        vals[2] = static_cast<jvalue>(JniHolder(&env, t2));
        vals[3] = static_cast<jvalue>(JniHolder(&env, t3));
        CallRealJni(&env, 0, 1, vals);
        }

    template <typename... Args>
    void CallToJni(Args... args)
        {
        std::cout << "Wow: " << sizeof...(args) << "\n";
        JNIEnv env;
        const int size = sizeof...(args);
        jvalue vals[size] = { (static_cast<jvalue>(JniHolder(&env, args)), 0)... };
        CallRealJni(&env, 0, 1, vals);
        }

    ///////////////////////////////////////////////////////////

    template <typename MethodType>
    struct Impl
        {
        template <typename... Args>
        static MethodType CallMethod(Args... args);

        template <typename... Args>
        static void CallStaticMethod(Args... args);
        };

    template <>
    struct Impl <void>
        {
        template <typename... Args>
        static void CallMethod(Args... args)
            {
            std::cout << "void method\n";
            CallToJni(args...);
            return;
            }
        };

    template <>
    struct Impl <int>
        {
        template <typename... Args>
        static int CallMethod(Args... args)
            {
            std::cout << "int method\n";
            CallToJni(args...);
            return 1;
            }
        };

    // two possible variants
    //  1. overload and typical specialization for methods returning void|string|float|int...
    //  2. dynamic configuration as for type structure
    //  preferred 1st
    template <typename MethodType, typename... Args>
    MethodType CallMethodImpl(const char* className, const char* mname, std::string&& signature, Args... args)
       {
       std::cout << "Signature: " << signature << std::endl;
       return Impl<MethodType>::CallMethod(args...);
       }

    template <typename MethodType, typename... Args>
    MethodType CallMethod(const char* className, const char* mname, Args... args);

    template <typename MethodType, typename... Args>
    MethodType CallMethod(const char* className, const char* mname, Args... args)
        {
        std::string signature_string = "(";
        GetType(signature_string, args...);
        signature_string += ")";

        signature_string += GetTypeName<MethodType>();


        std::cout << "Signature: " << signature_string << std::endl;
        return Impl<MethodType>::CallMethod(args...);
        }

    } // utils

/////////

int _tmain(int argc, _TCHAR* argv[])
    {
    std::cout << "0 parameters\n";
    utils::CallMethod<void>("a", "b");
    
    std::cout << "\n1 parameter\n";
    utils::CallMethod<void>("a", "b", 1);
    std::cout << "\n2 parameters\n";
    utils::CallMethod<void>("a", "b", 1, "s");
    std::cout << "\n3 parameters\n";
    int a = utils::CallMethod<int>("a", "b", 1, "s", 1.3f);

    std::cout << "\n3 parameters\n";
    utils::CallMethod<void>("a", "b", 1, "s", 1.3);

    std::cout << "\n6 parameters\n";
    utils::CallMethod<void>("a", "b", 1, "s", 1.3, true, '5', 1.f);
    return 0;
    }

