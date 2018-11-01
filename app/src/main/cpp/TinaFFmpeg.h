//
// Created by xiucheng yin on 2018/10/26.
//

#ifndef TINAPLAYER_TINAFFMPEG_H
#define TINAPLAYER_TINAFFMPEG_H

#include "JavaCallHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"

extern "C" {
#include <libavformat/avformat.h>
}


class TinaFFmpeg {
public:
    TinaFFmpeg(JavaCallHelper *javaCallHelper, const char *dataSource);

    ~TinaFFmpeg();

    void prepare();

    void _prepare();

    void start();

    void _start();

    void setRenderFrameCallback(RenderFrameCallback callback);

    void stop();

public:
    char *dataSource;
    pthread_t pid;
    pthread_t pid_play;
    AVFormatContext *formatContext = 0;
    JavaCallHelper *callHelper;
    AudioChannel *audioChannel = 0;//指针初始化最好赋值为null
    VideoChannel *videoChannel = 0;
    bool isPlaying;
    RenderFrameCallback callback;
    pthread_t pid_stop;
};


#endif //TINAPLAYER_TINAFFMPEG_H
