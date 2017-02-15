package com.umeng.social;

import android.app.Application;

import com.umeng.socialize.Config;
import com.umeng.socialize.PlatformConfig;
import com.umeng.socialize.UMShareAPI;

/**
 * Created by wsli on 17/2/15.
 */

public class GameApp extends Application {
    {
        PlatformConfig.setWeixin("wx2db3ffa3b2baa12a", "d517d1e027c8ba54bb9da54660d49ebb");
    }
    @Override
    public void onCreate() {
        super.onCreate();
        //开启debug模式，方便定位错误，具体错误检查方式可以查看http://dev.umeng.com/social/android/quick-integration的报错必看，正式发布，请关闭该模式
        Config.DEBUG = true;
        UMShareAPI.get(this);
    }
}
