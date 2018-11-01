package com.tina.live;

import com.tina.live.list.LiveList;
import com.tina.live.room.Room;
import com.google.gson.Gson;

import java.io.IOException;

import io.reactivex.Flowable;
import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.Response;
import retrofit2.Retrofit;
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory;
import retrofit2.converter.gson.GsonConverterFactory;

/**
 * @author Lance
 * @date 2018/9/7
 */
public class LiveManager {

    private final LiveService liveService;
    private static LiveManager instance;

    private LiveManager() {
        OkHttpClient callFactory = new OkHttpClient.Builder().addInterceptor(new Interceptor() {
            @Override
            public Response intercept(Chain chain) throws IOException {
                Response response = chain.proceed(chain.request());
                LiveList liveList = new Gson().fromJson(response.body().string(), LiveList.class);
                System.out.println(liveList.getData().getItems().get(0).getPictures().getImg());
                return response;
            }
        }).build();
        Retrofit retrofit = new Retrofit.Builder().baseUrl("http://api.m.panda.tv/")
                .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
                .addConverterFactory(GsonConverterFactory.create())
                .build();
        liveService = retrofit.create(LiveService.class);
    }

    public static LiveManager getInstance() {
        if (null == instance) {
            synchronized (LiveService.class) {
                if (null == instance) {
                    instance = new LiveManager();
                }
            }
        }
        return instance;
    }


    public Flowable<LiveList> getLiveList(String cate) {
        return liveService.getLiveList(cate, 1, 10, "3.3.1.5978");
    }

    public Flowable<Room> getLiveRoom(String id) {
        return liveService.getLiveRoom(id, "3.3.1.5978", 1, "json", "android");
    }
}
