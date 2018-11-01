//
// Created by xiucheng yin on 2018/10/27.
//

#ifndef TINAPLAYER_BASECHANNEL_H
#define TINAPLAYER_BASECHANNEL_H

#include "safe_queue.h"
extern "C"{
#include <libavcodec/avcodec.h>
};

class BaseChannel{
public:
    BaseChannel(int id, AVCodecContext *avCodecContext, AVRational time_base):id(id), avCodecContext(avCodecContext), time_base(time_base){
        packets.setRelaseCallback(releaseAvPacket);
        frames.setRelaseCallback(releaseAvFrame);
    }

    virtual ~BaseChannel(){
        packets.clear();
        frames.clear();
        if (avCodecContext){
            avcodec_close(avCodecContext);
            avcodec_free_context(&avCodecContext);
            avCodecContext = 0;
        }
    }//子类必须实现

    static void releaseAvPacket(AVPacket** packet){
        if (packet){
            av_packet_free(packet);
            *packet = 0;
        }
    }

    static void releaseAvFrame(AVFrame** aVFrame){
        if (aVFrame){
            av_frame_free(aVFrame);
            *aVFrame = 0;
        }
    }

    virtual void play() = 0;

    virtual void stop() = 0;

    int id;
    //解码数据包队列
    SafeQueue<AVFrame *> frames;
    //编码数据包队列
    SafeQueue<AVPacket*> packets;

    AVCodecContext *avCodecContext;

    bool isPlaying;

    AVRational time_base;
public:
    double clock;
};

#endif //TINAPLAYER_BASECHANNEL_H
