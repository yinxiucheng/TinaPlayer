package tina.com.player;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * @author yxc
 * @date 2018/10/26
 */
public class TinaPlayer implements SurfaceHolder.Callback{

    static {
        System.loadLibrary("native-lib");
    }

    private String dataSource;

    private SurfaceHolder holder;

    private OnPrepareListener listener;
    /**
     * 设置视频源
     * @param dataSource
     */
    public void setDataSource(String dataSource){
        this.dataSource = dataSource;
    }

    /**
     * 设置播放显示的画布
     *
     * @param surfaceView
     */
    public void setSurfaceView(SurfaceView surfaceView){
        if (null != holder){
            holder.removeCallback(this);
        }
        holder = surfaceView.getHolder();
        holder.addCallback(this);
    }

    /**
     * 准备播放视频
     */
    public void prepare(){
        native_prepare(dataSource);
    }

    /**
     * 播放
     */
    public void start(){
        native_start();
    }

    public void stop(){
        native_stop();
    }

    public void onError(int errorCode){
        System.out.println("Java接到回调:"+errorCode);
    }


    public void onPrepare(){
        if (null != listener){
            listener.onPrepare();
        }
    }

    public void setOnPrepareListener(OnPrepareListener onPrepareListener){
        this.listener = onPrepareListener;
    }

    public interface OnPrepareListener{
        void onPrepare();
    }

    public void release(){
        holder.removeCallback(this);
        native_release();

    }

    /**
     * 画布创建OK
     *
     * @param holder
     */
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        native_setSurface(holder.getSurface());
    }

    /**
     * 画布发生变化（横竖屏）
     * @param holder
     * @param format
     * @param width
     * @param height
     */
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        native_setSurface(holder.getSurface());
    }

    /**
     * 画布销毁 （home， 退出应用）。画布不可见
     * @param holder
     */
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    native void native_prepare(String dataSource);

    native void native_start();

    native void native_stop();

    native void native_release();

    native void native_setSurface(Surface surface);


}
