package cn.gloud.client.zxing.view;


import java.io.IOException;
import java.lang.ref.WeakReference;
import java.util.Hashtable;
import java.util.Vector;

import cn.gloud.client.zxing.camera.CameraManager;
import cn.gloud.client.zxing.decoding.CaptureActivityHandler;
import cn.gloud.client.zxing.decoding.InactivityTimer;
import cn.gloud.client.zxing.image.RGBLuminanceSource;

import com.gloud.phonehelper.R;
import com.google.zxing.BarcodeFormat;
import com.google.zxing.BinaryBitmap;
import com.google.zxing.ChecksumException;
import com.google.zxing.DecodeHintType;
import com.google.zxing.FormatException;
import com.google.zxing.NotFoundException;
import com.google.zxing.Result;
import com.google.zxing.common.HybridBinarizer;
import com.google.zxing.qrcode.QRCodeReader;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.res.AssetFileDescriptor;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import android.view.View;


public class ScanQrActivity extends Activity implements Callback, View.OnClickListener{

	private CaptureActivityHandler handler;
	private ViewfinderView viewfinderView;
	private boolean hasSurface;
	private Vector<BarcodeFormat> decodeFormats;
	private String characterSet;
	private InactivityTimer inactivityTimer;
	private MediaPlayer mediaPlayer;
	private boolean playBeep;
	private static final float BEEP_VOLUME = 0.10f;
	private boolean vibrate;
	
	
	private static final int REQUEST_CODE = 100;
	private static final int PARSE_BARCODE_SUC = 0;
	private static final int PARSE_BARCODE_FAIL = -1;
	private ProgressDialog mProgress;
	private String photo_path;
	private Bitmap scanBitmap;
	private boolean isFlashlightOpen;
	
	public static native void JniCreateVgcScene(int resultParse, String strResult);

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_capture);
		CameraManager.init(getApplication());
		viewfinderView = (ViewfinderView) findViewById(R.id.viewfinder_view);
		findViewById(R.id.capture_flashlight).setOnClickListener(this);
		findViewById(R.id.capture_scan_photo).setOnClickListener(this);
		
		hasSurface = false;
		inactivityTimer = new InactivityTimer(this);	
		isFlashlightOpen = false;	
	}		

	//初始化摄像头
	@Override
	protected void onResume() {
		super.onResume();
		SurfaceView surfaceView = (SurfaceView) findViewById(R.id.preview_view);
		SurfaceHolder surfaceHolder = surfaceView.getHolder();
		if (hasSurface) {
			initCamera(surfaceHolder);
		} else {
			surfaceHolder.addCallback(this);//ScanQrActivity实现其callback接口
			//surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		}
		decodeFormats = null;
		characterSet = null;

		playBeep = true;
		AudioManager audioService = (AudioManager) getSystemService(AUDIO_SERVICE);
		if (audioService.getRingerMode() != AudioManager.RINGER_MODE_NORMAL) {
			playBeep = false;
		}
		initBeepSound();
		vibrate = true;
		
	}

	@Override
	protected void onPause() {
		super.onPause();
		if (handler != null) {
			handler.quitSynchronously();
			handler = null;
		}
		CameraManager.get().closeDriver();
	}

	@Override
	protected void onDestroy() {
		inactivityTimer.shutdown();
		super.onDestroy();
	}

	//initCamera
	private void initCamera(SurfaceHolder surfaceHolder) {
		try {
			CameraManager.get().openDriver(surfaceHolder);
		} catch (IOException ioe) {
			return;
		} catch (RuntimeException e) {
			return;
		}
		
		if (handler == null) {
			handler = new CaptureActivityHandler(this, decodeFormats,characterSet);//用于进行扫描解码处理
		}
	}

	//SurfaceHolder接口实现
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {

	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		if (!hasSurface) {
			hasSurface = true;
			initCamera(holder);
		}

	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		hasSurface = false;

	}

	public ViewfinderView getViewfinderView() {
		return viewfinderView;
	}

	public Handler getHandler() {
		return handler;
	}

	public void drawViewfinder() {
		viewfinderView.drawViewfinder();

	}

	/**
	 * 处理扫描结果
	 * @param result
	 * @param barcode
	 */
	public void handleDecode(Result result, Bitmap barcode) {
		Log.d("ScanQrActivity", "handleDecode(...)called");
		inactivityTimer.onActivity();
		playBeepSoundAndVibrate();
		onResultHandler(null == barcode ? PARSE_BARCODE_FAIL:PARSE_BARCODE_SUC, result.getText(), barcode);
	}
	
	@Override
	public void onClick(View v) {
		switch (v.getId()) {
			case R.id.capture_scan_photo: 
				Intent innerIntent = new Intent(Intent.ACTION_GET_CONTENT); // "android.intent.action.GET_CONTENT"
				innerIntent.setType("image/*");
				Intent wrapperIntent = Intent.createChooser(innerIntent,getResources().getText(R.string.scan_image_choose));
				this.startActivityForResult(wrapperIntent, REQUEST_CODE);
				break;

			case R.id.capture_flashlight:
				if (isFlashlightOpen) {
					CameraManager.get().openLight(); 
					isFlashlightOpen = false;
				}
				else {
					CameraManager.get().offLight(); 
					isFlashlightOpen = true;
				}
				break;
			default:
				break;
		}

	}
	
	static class MyHandler extends Handler {  
        WeakReference<ScanQrActivity> mActivity; 
        MyHandler(ScanQrActivity activity) {  
                mActivity = new WeakReference<ScanQrActivity>(activity);  
        }  

        @Override  
        public void handleMessage(Message msg) {  
        	ScanQrActivity theActivity = mActivity.get();  		
        	theActivity.mProgress.dismiss();
                switch (msg.what) {  
    			case PARSE_BARCODE_SUC:
    				Log.d("ScanQrActivity", "handleMessage(...)called");
    				theActivity.onResultHandler(PARSE_BARCODE_SUC, (String)msg.obj, theActivity.scanBitmap);
    				break;
    			case PARSE_BARCODE_FAIL:
    				theActivity.onResultHandler(PARSE_BARCODE_FAIL, (String)msg.obj, null);
    				break;
                }  
        }  
}
	MyHandler mHandler = new MyHandler(this);	

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if(resultCode == RESULT_OK){
			switch(requestCode){
			case REQUEST_CODE:
				//获取选中图片的路径
				Cursor cursor = getContentResolver().query(data.getData(), null, null, null, null);
				if (cursor.moveToFirst()) {
					photo_path = cursor.getString(cursor.getColumnIndex(MediaStore.Images.Media.DATA));
				}
				cursor.close();
				
				mProgress = new ProgressDialog(ScanQrActivity.this);
				mProgress.setMessage(getResources().getText(R.string.scan_capturing));
				mProgress.setCancelable(false);
				mProgress.show();
				
				new Thread(new Runnable() {
					@Override
					public void run() {
						Result result = scanningImage(photo_path);
						if (result != null) {
							Message m = mHandler.obtainMessage();
							m.what = PARSE_BARCODE_SUC;
							m.obj = result.getText();
							mHandler.sendMessage(m);
						} else {
							Message m = mHandler.obtainMessage();
							m.what = PARSE_BARCODE_FAIL;
							m.obj = "Scan failed!";
							mHandler.sendMessage(m);
						}
					}
				}).start();
				
				break;
			
			}
		}
	}
	
	/**
	 * 扫描二维码图片的方法
	 * @param path
	 * @return
	 */
	public Result scanningImage(String path) {
		if(TextUtils.isEmpty(path)){
			return null;
		}
		Hashtable<DecodeHintType, String> hints = new Hashtable<DecodeHintType, String>();
		hints.put(DecodeHintType.CHARACTER_SET, "utf-8"); //设置二维码内容的编码

		BitmapFactory.Options options = new BitmapFactory.Options();
		options.inJustDecodeBounds = true; // 先获取原大小
		scanBitmap = BitmapFactory.decodeFile(path, options);
		options.inJustDecodeBounds = false; // 获取新的大小
		int sampleSize = (int) (options.outHeight / (float) 200);
		if (sampleSize <= 0)
			sampleSize = 1;
		options.inSampleSize = sampleSize;
		scanBitmap = BitmapFactory.decodeFile(path, options);
		RGBLuminanceSource source = new RGBLuminanceSource(scanBitmap);
		BinaryBitmap bitmap1 = new BinaryBitmap(new HybridBinarizer(source));
		QRCodeReader reader = new QRCodeReader();
		try {
			return reader.decode(bitmap1, hints);

		} catch (NotFoundException e) {
			e.printStackTrace();
		} catch (ChecksumException e) {
			e.printStackTrace();
		} catch (FormatException e) {
			e.printStackTrace();
		}
		return null;
	}
	

	
	
	/**
	 * 跳转到上一个页面
	 * @param resultString
	 * @param bitmap
	 */
	//resultString : "GloudGame#10_0_1_132#9627#MAGICBOX";
	private void onResultHandler(int resultParse, String resultString, Bitmap bitmap){
		if(TextUtils.isEmpty(resultString)){
			resultParse = PARSE_BARCODE_FAIL;
		}
		if(resultParse == PARSE_BARCODE_SUC)
		{
			Log.d("ScanQrActivity", "result ===> " + resultString);
			
		}
		JniCreateVgcScene(resultParse, resultString);
		ScanQrActivity.this.finish();
	}

	private void initBeepSound() {
		if (playBeep && mediaPlayer == null) {
			// The volume on STREAM_SYSTEM is not adjustable, and users found it
			// too loud,
			// so we now play on the music stream.
			setVolumeControlStream(AudioManager.STREAM_MUSIC);
			mediaPlayer = new MediaPlayer();
			mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
			mediaPlayer.setOnCompletionListener(beepListener);

			AssetFileDescriptor file = getResources().openRawResourceFd(R.raw.beep);
			try {
				mediaPlayer.setDataSource(file.getFileDescriptor(),
						file.getStartOffset(), file.getLength());
				file.close();
				mediaPlayer.setVolume(BEEP_VOLUME, BEEP_VOLUME);
				mediaPlayer.prepare();
			} catch (IOException e) {
				mediaPlayer = null;
			}
		}
	}

	private static final long VIBRATE_DURATION = 200L;
	private void playBeepSoundAndVibrate() {
		if (playBeep && mediaPlayer != null) {
			Log.d("ScanQrActivity", "mediaPlayer.start()");
			mediaPlayer.start();
		}
		if (vibrate) {
			Vibrator vibrator = (Vibrator) getSystemService(VIBRATOR_SERVICE);
			vibrator.vibrate(VIBRATE_DURATION);
		}
	}

	/**
	 * When the beep has finished playing, rewind to queue up another one.
	 */
	private final OnCompletionListener beepListener = new OnCompletionListener() {
		public void onCompletion(MediaPlayer mediaPlayer) {
			mediaPlayer.seekTo(0);
		}
	};

}
