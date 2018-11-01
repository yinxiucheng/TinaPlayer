//
// Created by xiucheng yin on 2018/10/26.
//

#ifndef TINAPLAYER_JAVACALLHELPER_H
#define TINAPLAYER_JAVACALLHELPER_H

#include <jni.h>

class JavaCallHelper {
public:
    JavaCallHelper(JavaVM *vm,JNIEnv* env,jobject instance);
    ~JavaCallHelper();
    //回调java
    void  onError(int thread,int errorCode);
    void onPrepare(int thread);
private:
    JavaVM *vm;
    JNIEnv *env;
    jobject  instance;
    jmethodID onErrorId;
    jmethodID onPrepareId;
};

#endif //TINAPLAYER_JAVACALLHELPER_H
