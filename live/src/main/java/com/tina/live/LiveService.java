package com.tina.live;

import com.tina.live.list.LiveList;
import com.tina.live.room.Room;

import io.reactivex.Flowable;
import retrofit2.http.GET;
import retrofit2.http.Query;

/**
 * @author Lance
 * @date 2018/9/7
 */
public interface LiveService {

    @GET("ajax_get_live_list_by_cate")
    Flowable<LiveList> getLiveList(@Query("cate") String cate, @Query("pageno") int pageno, @Query
            ("pagenum") int pagenum, @Query("version") String version);

    @GET("ajax_get_liveroom_baseinfo")
    Flowable<Room> getLiveRoom(@Query("roomid") String roomid, @Query("__version") String
            __version, @Query("slaveflag") int slaveflag, @Query("type") String type, @Query
                                       ("__plat") String __plat);
}
