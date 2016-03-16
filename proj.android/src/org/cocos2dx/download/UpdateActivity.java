package org.cocos2dx.download;

import org.cocos2dx.cpp.AppActivity;

import com.gloud.phonehelper.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;
 
/****************************
 * 鍒涘缓鏃堕棿锛�2014骞�5鏈�19鏃� 涓婂崍11:21:55  
 * 椤圭洰鍚嶇О锛欸loud  
 * @author 璧靛己
 * @version 1.0   
 * @since JDK 1.8.0
 * 鏂囦欢鍚嶇О锛歎pdateActivity.java  
 * 绫昏鏄庯細  鏇存柊
 ****************************/

public class UpdateActivity extends Activity implements Constant{
	
	private TextView mCurrentTv;
	
	private TextView mProgressTv;
	
	private ProgressBar mUpdateBar;
	
	private UpdateProgress mUpdateProgress;
	
//	private ImageView mAlplaImage;
//	private Animation mIconAnim;
	private ImageView mIcon;
	//private String GLOUD_CLIENT_URL=NEW_CLIENT_URL;
	private String mUrl="";
	private String downloadType = "";
	private UpdateUtils mUpdateUtils;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//璁╁睆骞曚笉鍘讳紤鐪�
		//GLOUD_CLIENT_URL=String.format(DOWN_LOAD_GLOUDCLIENT_URL, AppActivity.GenerateUUID(UpdateActivity.this),AppActivity.getChannel(UpdateActivity.this));
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); 
		setContentView(R.layout.activity_update);
		downloadType = getIntent().getStringExtra(DOWNLOAD_TYPE);	
		//灏忔牸鍔╂墜鍗囩骇
		if(downloadType.equals(DOWNLOAD_GLOUD_HELPER))
		{
			mUrl=getIntent().getStringExtra(URL_STR).trim();
		}
		else
		{
			//鏍兼潵浜戝ぇ鍘�
			mUrl=String.format(DOWN_LOAD_GLOUDCLIENT_URL, AppActivity.GenerateUUID(UpdateActivity.this));
		}
		Log.d("UpdateActivity", "downApk URL = "+ mUrl);
		init();
		mUpdateUtils=new UpdateUtils(this, null);
    	mUpdateUtils.downApk(mUrl);
	}

	@Override
	protected void onResume() {
		super.onResume();
	}
	
	@Override
	protected void onPause() {
		super.onPause();
	}
	

	private void init()
	{
		mUpdateProgress=new UpdateProgress();
		
		mCurrentTv=(TextView) findViewById(R.id.current_tv);
		
		mProgressTv=(TextView) findViewById(R.id.progress_tv);
		
		mIcon=(ImageView) findViewById(R.id.icon_img);
		
		mUpdateBar=(ProgressBar) findViewById(R.id.update_progressBar);
		
		IntentFilter filter=new IntentFilter();
		filter.addAction(UPDATE_PROGRESS_ACTION);
		registerReceiver(mUpdateProgress, filter);
		if(downloadType.equals(DOWNLOAD_GLOUD_GAME)) {
			mIcon.setImageResource(R.drawable.gloudclient_icln);
			mCurrentTv.setText(R.string.download_client);			
		}
		else 
		{
			mIcon.setImageResource(R.drawable.gloud_helper_icon);
			mCurrentTv.setText(R.string.download_helper);
		}
	}
	public static String getVersionName(Context context) {
		String versionName = "";
		try {
			versionName = context.getPackageManager().getPackageInfo(
					context.getPackageName(), 0).versionName;
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return versionName;
	}

	@Override
	protected void onDestroy() {
		unregisterReceiver(mUpdateProgress);
		super.onDestroy();
		if (mUrl.equals(NEW_CLIENT_URL)) {
			mCurrentTv.setText(R.string.download_client);
			//UMGameAgent.finishLevel("3-downloadGloudClient");				
		}else {
			mCurrentTv.setText(R.string.download_helper);
			//UMGameAgent.finishLevel("2-upload");
		}
	}
	class UpdateProgress extends BroadcastReceiver{
		@Override
		public void onReceive(Context context, Intent intent) {
			// TODO Auto-generated method stub
			boolean uploadFail=intent.getBooleanExtra(UPDATE_FAIL, false);
			if (!uploadFail) {				
				float progress=intent.getFloatExtra(UPDATE_PROGRESS, 100.0f);
				progress = (float)(Math.round(progress*100)/100.0);//保留2位小数
				Log.d("progress", "progress = "+progress);
				mProgressTv.setText(progress+"%");
				int Progress=(int)progress;
				mUpdateBar.setProgress(Progress);
				if (100==Progress) {
					UpdateActivity.this.finish();
				}
			}else {
				//鏇存柊澶辫触
				mProgressTv.setText(getString(R.string.update_fail));
				Toast.makeText(UpdateActivity.this, R.string.update_fail, Toast.LENGTH_SHORT).show();
			}
		}
		
	}
	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
//		if (!mUpdateing) {
			if (event.getKeyCode()==KeyEvent.KEYCODE_BACK) {
				dialogExit();
				return true;
			}
//		}
		return super.onKeyUp(keyCode, event);
	}
	
	private void dialogExit()
	{
		AlertDialog.Builder builder = new Builder(this);
		builder.setTitle(R.string.title_exit_update);
		builder.setMessage(R.string.text_exit_update);
		builder.setPositiveButton(R.string.button_cancel, new OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				// TODO Auto-generated method stub
				dialog.dismiss();
			}
		});
		builder.setNegativeButton(R.string.button_ok, new OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				// TODO Auto-generated method stub
				dialog.dismiss();
				mUpdateUtils.stopDownload();
				UpdateActivity.this.finish();
				
			}
		});
		builder.create().show();
	}
}
  
