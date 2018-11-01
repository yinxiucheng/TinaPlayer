package tina.com.player;

import android.content.res.Configuration;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.DisplayMetrics;
import android.view.SurfaceView;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Toast;

import com.trello.rxlifecycle2.components.support.RxAppCompatActivity;

/**
 * @author Lance
 * @date 2018/9/7
 */
public class PlayActivity extends RxAppCompatActivity {
    private TinaPlayer tinaPlayer;
    public String url;

    SurfaceView surfaceView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager
                .LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_play);
        surfaceView = findViewById(R.id.surfaceView);

        setLayoutParamsForSurfaceView();
        tinaPlayer = new TinaPlayer();
        tinaPlayer.setSurfaceView(surfaceView);
        tinaPlayer.setOnPrepareListener(new TinaPlayer.OnPrepareListener() {

            @Override
            public void onPrepare() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(PlayActivity.this, "开始播放", Toast.LENGTH_SHORT).show();
                    }
                });
                tinaPlayer.start();
            }
        });

        url = getIntent().getStringExtra("url");
        tinaPlayer.setDataSource(url);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        setLayoutParamsForSurfaceView();
    }

    public void setLayoutParamsForSurfaceView(){
        DisplayMetrics dm = new DisplayMetrics();
        if (isOrientationPortrait()) {
            getWindowManager().getDefaultDisplay().getMetrics(dm);
            int screenWidth = dm.widthPixels;
            ViewGroup.LayoutParams lp = surfaceView.getLayoutParams();
            lp.width = screenWidth;
            lp.height = screenWidth * 9/16;
            surfaceView.setLayoutParams(lp);
        } else if (isOrientationLandscape()) {
            //横屏显示
            ViewGroup.LayoutParams lp = surfaceView.getLayoutParams();
            lp.width = lp.MATCH_PARENT;
            lp.height = lp.MATCH_PARENT;
            surfaceView.setLayoutParams(lp);
        }
    }

    public int getConfiguration(){
        if (null != getResources() && null != getResources().getConfiguration()){
            return getResources().getConfiguration().orientation;
        }
        return -1;
    }

    public boolean isOrientationPortrait(){
        return  getConfiguration() == Configuration.ORIENTATION_PORTRAIT;
    }

    public boolean isOrientationLandscape(){
        return getConfiguration() == Configuration.ORIENTATION_LANDSCAPE;
    }

    @Override
    protected void onResume() {
        super.onResume();
        tinaPlayer.prepare();
    }

    @Override
    protected void onStop() {
        super.onStop();
        tinaPlayer.stop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        tinaPlayer.release();
    }
}
