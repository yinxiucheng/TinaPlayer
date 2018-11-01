//
// Created by xiucheng yin on 2018/10/26.
//

#ifndef TINAPLAYER_AUDIOCHANNEL_H
#define TINAPLAYER_AUDIOCHANNEL_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "BaseChannel.h"

extern "C" {
#include <libswresample/swresample.h>
};

class AudioChannel : public BaseChannel {
public:
    AudioChannel(int id, AVCodecContext *avCodecContext, AVRational time_base);

    ~AudioChannel();

    void play();

    void decode();

    void _play();//播放，对应视频的渲染render

    void stop();

    int getPcm();


    uint8_t *data = 0;
    int out_channels;
    int out_samplesize;
    int out_sample_rate;

private:
    pthread_t pid_audio_decode;
    pthread_t pid_audio_player;//播放线程

    /**
     * OpenSL ES
     */
    //引擎
    SLObjectItf engineObject = 0;
    //引擎接口
    SLEngineItf engineInterface = 0;

    //混音器
    SLObjectItf outputMixObject = 0;

    //播放器
    SLObjectItf bqPlayerObject = 0;

    //播放器接口
    SLPlayItf bqPlayerInterface = 0;

    //队列结构
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueueInterface = 0;


    SwrContext *swrContext = 0;


};


#endif //TINAPLAYER_AUDIOCHANNEL_H
