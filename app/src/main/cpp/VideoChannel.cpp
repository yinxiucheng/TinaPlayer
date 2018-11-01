//
// Created by xiucheng yin on 2018/10/26.
//
#include "VideoChannel.h"
#include "macro.h"

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
}

//丢包，知道下一个关键帧
void dropAvPacket(queue<AVPacket *> &q) {
    while (!q.empty()) {
        AVPacket *packet = q.front();
        if (packet->flags != AV_PKT_FLAG_KEY) {
            BaseChannel::releaseAvPacket(&packet);
            q.pop();
        } else {

            break;
        }
    }
}

//丢帧
void dropAvFramev(queue<AVFrame *> &q) {
    if (!q.empty()) {
        AVFrame *frame = q.front();
        BaseChannel::releaseAvFrame(&frame);
        q.pop();
    }
}

VideoChannel::VideoChannel(int id, AVCodecContext *avCodecContext, AVRational time_base, int fps)
        : BaseChannel(id, avCodecContext, time_base) {
    this->fps = fps;
    //用于同步操作队列的一个函数指针
//    packets.setSyncHandle(dropAvPacket);

    //丢画面
    frames.setSyncHandle(dropAvFramev);
}

VideoChannel::~VideoChannel() {
    frames.clear();
}

void *decode_task(void *args) {
    VideoChannel *channel = static_cast<VideoChannel *>(args);
    channel->decode();
    return 0;
}


void *render_task(void *args) {
    VideoChannel *channel = static_cast<VideoChannel *>(args);
    channel->render();
    return 0;
}

void VideoChannel::setAudioChannel(AudioChannel *audioChannel) {
    this->audioChannel = audioChannel;
}

void VideoChannel::play() {
    isPlaying = 1;
    packets.setWork(1);
    frames.setWork(1);

    //1.解码
    pthread_create(&pid_decode, 0, decode_task, this);

    //2. 播放
    pthread_create(&pid_render, 0, render_task, this);


}

//解码
void VideoChannel::decode() {
    AVPacket *packet = 0;
    while (isPlaying) {
        int ret = packets.pop(packet);
        if (!isPlaying) {
            break;
        }
        if (!ret) {
            continue;
        }
        //把包丢给解码器
        ret = avcodec_send_packet(avCodecContext, packet);
        releaseAvPacket(&packet);
        while (ret != 0) {
            break;
        }
        AVFrame *frame = av_frame_alloc();

        avcodec_receive_frame(avCodecContext, frame);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if (ret != 0) {
            break;
        }
        //再开一个线程 播放。
        frames.push(frame);
    }
    releaseAvPacket(&packet);
}

//渲染
void VideoChannel::render() {
    //目标： RGBA, 没有缩放
    swsContext = sws_getContext(avCodecContext->width, avCodecContext->height,
                                avCodecContext->pix_fmt,
                                avCodecContext->width, avCodecContext->height, AV_PIX_FMT_RGBA,
                                SWS_BILINEAR, 0, 0, 0);
    //每个画面 刷新的间隔 单位：秒
    double frame_delays = 1.0 / fps;
    AVFrame *frame = 0;

    uint8_t *dst_data[4];
    int dst_linesize[4];
    av_image_alloc(dst_data, dst_linesize, avCodecContext->width, avCodecContext->height,
                   AV_PIX_FMT_RGBA, 1);
    while (isPlaying) {
        int ret = frames.pop(frame);
        if (!isPlaying) {
            break;
        }
        //src_lines: 每一行存放的 字节长度
        sws_scale(swsContext, reinterpret_cast<const uint8_t *const *>(frame->data),
                  frame->linesize, 0,
                  avCodecContext->height,
                  dst_data, dst_linesize);

        //获取当前 一个画面播放的时间
        double clock = frame->best_effort_timestamp * av_q2d(time_base);

        double extra_delay = frame->repeat_pict / (2 * fps);
        double delays = extra_delay + frame_delays;
        if (!audioChannel) {
            av_usleep(delays * 1000000);
        } else if (clock == 0) {
            //休眠
            av_usleep(delays * 1000000);
        } else {
            //比较音频与视频
            double audioClock = audioChannel->clock;
            //间隔 音视频相差的间隔
            double diff = clock - audioClock;
            if (diff > 0) {
                //大于0 表示视频比较快
//                LOGE("视频快了：%lf", diff);
                av_usleep((delays + diff) * 1000000);
            } else if (diff < 0) {
                //不睡了，快点赶上音频
//                LOGE("音频快了：%lf",diff);
                // 视频包积压的太多了 （丢包）
                if (fabs(diff) >= 0.05) {
                    releaseAvFrame(&frame);
                    //丢包
                    frames.sync();
                    continue;
                }else{
                    //不睡了 快点赶上 音频
                }
            }
        }
        //把解出来的数据给到 window的Surface，回调出去进行播放
        callback(dst_data[0], dst_linesize[0], avCodecContext->width, avCodecContext->height);
        releaseAvFrame(&frame);
    }
    av_freep(&dst_data[0]);
    releaseAvFrame(&frame);
    isPlaying = 0;
    sws_freeContext(swsContext);
    swsContext = 0;
}

void VideoChannel::setRenderFrameCallback(RenderFrameCallback callback) {
    this->callback = callback;
}

void VideoChannel::stop() {
    isPlaying = 0;
    packets.setWork(0);
    frames.setWork(0);

    pthread_join(pid_decode, 0);
    pthread_join(pid_render, 0);
}


