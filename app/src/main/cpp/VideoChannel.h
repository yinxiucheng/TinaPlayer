//
// Created by xiucheng yin on 2018/10/26.
//

#ifndef TINAPLAYER_VIDEOCHANNEL_H
#define TINAPLAYER_VIDEOCHANNEL_H

#include "BaseChannel.h"
#include "AudioChannel.h"

extern "C" {
#include <libswscale/swscale.h>
};
typedef void (*RenderFrameCallback)(uint8_t *, int, int, int);
class VideoChannel : public BaseChannel {
public:
    VideoChannel(int id, AVCodecContext *avCodecContext, AVRational time_base, int fps);

    ~VideoChannel();

    void setAudioChannel(AudioChannel* audioChannel);

    void play();

    void decode();

    void render();

    void stop();

    void setRenderFrameCallback(RenderFrameCallback callback);

private:
    pthread_t pid_decode;
    pthread_t pid_render;

    SwsContext *swsContext = 0;
    RenderFrameCallback callback;
    int fps;//帧率

    AudioChannel *audioChannel = 0;
};


#endif //TINAPLAYER_VIDEOCHANNEL_H
