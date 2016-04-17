/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import java.util.List;
import java.util.UUID;

import org.cocos2dx.controller.Controller;
import org.cocos2dx.download.Constant;
import org.cocos2dx.download.UpdateActivity;
import org.cocos2dx.lib.Cocos2dxActivity;

import cn.gloud.client.zxing.view.ScanQrActivity;

import com.umeng.analytics.MobclickAgent;

import android.app.Activity;
import android.app.Application;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Vibrator;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.widget.Toast;

public class AppActivity extends Cocos2dxActivity implements Constant{

	public static Context context = null;
	private static String TAG = "AppActivity";
	private Controller mController;
	public static Application app = null;
	public static State wifiState = null;
    public static State mobileState = null;
    public static State ethernet =null;
	public enum NetWorkState {
    	ETHERNET,WIFI, MOBILE, NONE
	}
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);	
		context = AppActivity.this;
		app = getApplication();
//		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD) {
//			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
//		}
		//GloudHelper.getInstances(this);		
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		mController = new Controller();
//		SensorManager sm = (SensorManager) getSystemService(SENSOR_SERVICE);
//		List<Sensor> listSensor = sm.getSensorList(Sensor.TYPE_GRAVITY);
//		Sensor sensor = sm.getDefaultSensor(Sensor.TYPE_GRAVITY);
		
	}

	@Override
	protected void onResume() {
		super.onResume();
		MobclickAgent.onResume(this); 
	}

	@Override
	protected void onPause() {
		super.onPause();
		MobclickAgent.onPause(this);
		onVibrator(-1L);
	}
	@Override
	public void onContentChanged() {
		// TODO Auto-generated method stub
		super.onContentChanged();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if (mGLSurfaceView.isFocused()) {
			return mController.onKeyDown(keyCode, event);			
		}else if (mGLSurfaceView.getCocos2dxEditText().isFocused()) {
			return super.onKeyDown(keyCode, event);
		}
		return true;
	}
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if (mGLSurfaceView.isFocused() || keyCode == KeyEvent.KEYCODE_ENTER) {
			return mController.onKeyUp(keyCode, event);			
		} else if (mGLSurfaceView.getCocos2dxEditText().isFocused()) {
			return super.onKeyDown(keyCode, event);
		}
		return true;
	}
	@Override
	public boolean onGenericMotionEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		if (mGLSurfaceView.isFocused()) {
			return mController.onGenericMotionEvent(event);
		}
		else if (mGLSurfaceView.getCocos2dxEditText().isFocused()) {
			return super.onGenericMotionEvent(event);
		}

        // cocos2dx will handle the event
		return true;
	}
	public boolean onTouchEvent(MotionEvent event) {
		return true;
		// return super.onTouchEvent(event);
	}
	
	public boolean onKeyLongPress(int keyCode, KeyEvent event) {
		return true;
		// return super.onKeyLongPress(keyCode, event);
	}
	
	public boolean onKeyMultiple(int keyCode, int repeatCount, KeyEvent event) {
		return true;
		// return super.onKeyMultiple(keyCode, repeatCount, event);
	}
	
	public void onBackPressed() {
		super.onBackPressed();
	}
	
	public boolean onKeyShortcut(int keyCode, KeyEvent event) {
		return true;
		// return super.onKeyShortcut(keyCode, event);
	}
	
	public boolean onTrackballEvent(MotionEvent event) {
		return true;
		// return super.onTrackballEvent(event);
	}
	
//	public void onConfigurationChanged(Configuration newConfig) {
//		super.onConfigurationChanged(newConfig);
//		
//		if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
//			Log.d(TAG, "onConfigurationChanged(...)");
//		} else {
//
//		}
//	}
	
	/*
	 * 以下是jni调用
	 */
	//startJmDNS
	public static void startJmDNS()
	{
		new Thread(new Runnable()  
        {  
            @Override  
            public void run()  
            {  
                // TODO Auto-generated method stub  
                StartJmDNS startJmDNS = new StartJmDNS();
        		if (startJmDNS != null)
        		{	        			
        			startJmDNS.start();
        		}
            }  
        }).start();
		
	}
	
	
	// 鐢熸垚 device uuid
    public static String GenerateUUID(Context ctx) {
    	String deviceId = "";
    	
    	try {
	        final TelephonyManager tm = (TelephonyManager)ctx.getSystemService(Context.TELEPHONY_SERVICE);
	
	        final String tmDevice, tmSerial, androidId;
	        tmDevice = "" + tm.getDeviceId();
	        tmSerial = "" + tm.getSimSerialNumber();
	        androidId = "" + android.provider.Settings.Secure.getString(ctx.getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
	        Log.i("ZQ", "___deviceId:" + tmDevice + " SimSerialNumber:" + tmSerial +  " androidId:" + androidId);
	        
	        if (androidId.isEmpty() || androidId.length() < 10)
	        	return UUID.randomUUID().toString();
	
	        UUID deviceUuid = new UUID(androidId.hashCode(), ((long)tmDevice.hashCode() << 32) | tmSerial.hashCode());
	        deviceId = deviceUuid.toString();
	        
	        Log.d("uuid", deviceId);
    	}
    	catch (Exception e) {
    		e.printStackTrace();
        	return UUID.randomUUID().toString();
    	}
    	
    	return deviceId;
    }
    
    // logintoken
    public static void resetLoginToken(String token) {
        List<PackageInfo> packageInfos = context.getPackageManager().getInstalledPackages(PackageManager.GET_UNINSTALLED_PACKAGES);
        for(PackageInfo info:packageInfos){
            String packageName = info.packageName;
            if (packageName.contains("cn.gloud") && "cn.gloud".equals(info.sharedUserId)) {
                try {
                    Context ct=context.createPackageContext(packageName, Context.MODE_PRIVATE);
                    SharedPreferences sp=ct.getSharedPreferences("UserDefault", Context.MODE_PRIVATE);
                    SharedPreferences.Editor et=sp.edit();
                    et.putString("logintoken", token);
                    et.commit();
                } catch (PackageManager.NameNotFoundException e) {
                    e.printStackTrace();
                }
            }
        }
    }
    
    /**
     * * 下载大厅
     * */
    public static void InstallApk(final String url) {
    	((Activity)getContext()).runOnUiThread(new Runnable() {
			@Override
			public void run() {
		    	Intent update = new Intent(getContext(),UpdateActivity.class);
		    	update.putExtra(DOWNLOAD_TYPE, DOWNLOAD_GLOUD_GAME);
//		    	update.putExtra(NEW_VERSION, "");
//				update.putExtra(SHOWNAME, false);
				update.putExtra(URL_STR,String.format(DOWN_LOAD_GLOUDCLIENT_URL, GenerateUUID(getContext()),getChannel(getContext())));
				getContext().startActivity(update);
			}
		});
	}

    /**
     * * 下载小格助手
     * */
    public static void InstallHelperApk(final String url) {
    	((Activity)getContext()).runOnUiThread(new Runnable() {
			@Override
			public void run() {
		    	Intent update = new Intent(getContext(),UpdateActivity.class);
		    	update.putExtra(DOWNLOAD_TYPE, DOWNLOAD_GLOUD_HELPER);
//		    	update.putExtra(NEW_VERSION, "");
//				update.putExtra(SHOWNAME, false);
				update.putExtra(URL_STR, url);
				getContext().startActivity(update);
			}
		});
	}
    
    /**
	 * @return 渠道
	 * */
	public static String getChannel(Context ctx)
	{
//		try {
//			ApplicationInfo ai = ctx.getPackageManager().getApplicationInfo(
//					ctx.getPackageName(), PackageManager.GET_META_DATA);
//			return ai.metaData.get("UMENG_CHANNEL").toString();
//		} catch (NameNotFoundException e) {
//			e.printStackTrace();
//		}
		return "gloud";
	}
    
	
	public static void setupWifi() {
		Intent intent = new Intent(
				android.provider.Settings.ACTION_WIFI_SETTINGS);
		context.startActivity(intent);
	}

	// wifi name
	public static String getConnectWifiName() {
		WifiManager wifiManager = (WifiManager) context
				.getSystemService(WIFI_SERVICE);
		WifiInfo wifiInfo = wifiManager.getConnectionInfo();
		Log.d("wifiInfo", wifiInfo.toString());
		Log.d("SSID", wifiInfo.getSSID());
		return wifiInfo.getSSID();
	}
	 /**
     * 获取当前网络状态//NetWorkState{ETHERNET,WIFI, MOBILE, NONE}
     * @return
     */
	public static int getConnectState() {		
		ConnectivityManager manager = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		manager.getActiveNetworkInfo();
		try {
			wifiState = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState();
		}catch(StackOverflowError e){
		} 
		catch (Exception e) {
		}
		try {
			mobileState = manager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).getState();
		} catch (Exception e) {
		}
		try {
			ethernet = manager.getNetworkInfo(ConnectivityManager.TYPE_ETHERNET).getState();
		} catch (Exception e) {
		}
		

		if (null != ethernet&& State.CONNECTED == ethernet) {
			Log.d(TAG, "NetWorkState.ETHERNET");
			return 0;//return NetWorkState.ETHERNET;
		}
		if (null != wifiState && State.CONNECTED == wifiState) {

			Log.d(TAG, "NetWorkState.WIFI");
			return 1;//return NetWorkState.WIFI;
		} else if (null != ethernet && null != wifiState && null != mobileState
				&& State.CONNECTED != wifiState
				&& State.CONNECTED != mobileState
				&& State.CONNECTED != ethernet) {

			Log.d(TAG, "NetWorkState.NONE");
			return 3;//return NetWorkState.NONE;
		} else if (null!=mobileState && State.CONNECTED == mobileState) {

			Log.d(TAG, "NetWorkState.MOBILE");
			return 2;//return NetWorkState.MOBILE;
		}

		Log.d(TAG, "NetWorkState.NONE");
		return 3;//return NetWorkState.NONE;
	}
	public static boolean isEthernet()
	{
		ConnectivityManager connectivityManager = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo activeNetInfo = connectivityManager.getActiveNetworkInfo();
		if (activeNetInfo != null
				&& activeNetInfo.getType() == ConnectivityManager.TYPE_ETHERNET) {
			return true;
		}
		return false;		
	}

	/**
	 * @param 
	 * vibratorTime >0   Vibrator time, 
	 * vibratorTime <=0 cancel Vibrator
	 * 	      
	 */
	public static void onVibrator(final long milliseconds) {
		Vibrator vib = (Vibrator) context
				.getSystemService(Context.VIBRATOR_SERVICE);
//		Log.d(TAG, "milliseconds = "+milliseconds);
		if (null != vib) {
			if (milliseconds <= 0L) {
				vib.cancel();
			} else {
				vib.vibrate(milliseconds);
			}
		}
	} 
	//
	private final static String apkName = "cn.gloud.arena";
	private final static String className = "org.cocos2dx.cpp.AppActivity";
	public static boolean isInstalledApk() {
		PackageManager pm = context.getPackageManager();
		boolean installed = false;
		try {
			pm.getPackageInfo(apkName,PackageManager.GET_ACTIVITIES);
			installed = true;
		} catch(PackageManager.NameNotFoundException e) {
			installed = false;
		}
		return installed;
	}

	//
	public static void openApk() {
		Intent mIntent = new Intent( );   
		mIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);  
		ComponentName comp = new ComponentName(apkName, className);    
		mIntent.setComponent(comp);     
		mIntent.setAction("android.intent.action.VIEW");     
		context.startActivity(mIntent); 
	}
	
	public static void startApp()
	{
		Intent intent = context.getPackageManager().getLaunchIntentForPackage(apkName);
	    if(intent != null)
	   {
	    	context.startActivity(intent);
	   }
	   else
	   {
	       Toast.makeText(context, "启动失败，请重试", Toast.LENGTH_SHORT).show();
	   }
	}
	
	//当前版本
	public static int getClientVersionCode()
	{
		int versionCode = 0;
		try {
			versionCode = context.getPackageManager()
					.getPackageInfo(context.getPackageName(),
					0).versionCode;
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return versionCode;
	}

	//当前版本名
	public static String getClientVersionName()
	{
		try {
			return context.getPackageManager()
					.getPackageInfo(context.getPackageName(),
					0).versionName;
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return "";
	}
	

	public static void setScreenOrientation(int requestedOrientation)
	{
		//API level 18 以下使用ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE
		if(Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN_MR2 && requestedOrientation == ActivityInfo.SCREEN_ORIENTATION_USER_LANDSCAPE)
		{
			requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
		}
		((Activity) context).setRequestedOrientation(requestedOrientation);
		Log.d(TAG, "req dir = "+ requestedOrientation + ", dir = "+ ((Activity) context).getRequestedOrientation());
	}

	public static int getScreenOrientation()
	{
		return ((Activity) context).getRequestedOrientation();
	}
		
	public static void openSanQr()
	{
		Intent intent = new Intent();
		intent.setClass(context, ScanQrActivity.class);
		context.startActivity(intent);
	}

	public static String getPhoneModel()
	{
		return android.os.Build.MODEL;
	}
    
    /*
     * 读取联系人的信息
     */
    public static void ReadAllContacts()
    {
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.M &&
           checkSelfPermission(Manifest.permission.READ_CONTACTS) != PackageManager.PERMISSION_GRANTED)
        {
            requestPermissions(new String[]{Manifest.permission.READ_CONTACTS},
                               PERMISSIONS_REQUEST_CODE_ACCESS_READ_CONTACTS);
            //等待回调 onRequestPermissionsResult(int, String[], int[]) method
            
        }
        else
        {
            //获得授权，做相应的处理！
            ToReadAllContacts();
        }
    }
    
    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        if (requestCode == PERMISSIONS_REQUEST_CODE_ACCESS_READ_CONTACTS
            && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            // 获得授权后处理方法
            ToReadAllContacts();
        }
    }
    
    
    
    /*
     * 读取联系人的信息
     */
    public void ToReadAllContacts() {
        
        class ContactsData
        {
            String id;
            public String name;
            public Collection<String> telephone;
            public Collection<String> email;
        }
        
        Collection<ContactsData> collections = new ArrayList();
        
        
        Cursor cursor = getContentResolver().query(ContactsContract.Contacts.CONTENT_URI,
                                                   null, null, null, null);
        int contactIdIndex = 0;
        int nameIndex = 0;
        if(cursor.getCount() > 0) {
            contactIdIndex = cursor.getColumnIndex(ContactsContract.Contacts._ID);
            nameIndex = cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME);
        }
        while(cursor.moveToNext()) {
            ContactsData contactsData = new ContactsData();
            String contactId = cursor.getString(contactIdIndex);
            String name = cursor.getString(nameIndex);
            contactsData.id = contactId;
            contactsData.name = name;
            /*
             * 查找该联系人的phone信息
             */
            Cursor phones = getContentResolver().query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
                                                       null,
                                                       ContactsContract.CommonDataKinds.Phone.CONTACT_ID + "=" + contactId,
                                                       null, null);
            int phoneIndex = 0;
            if(phones.getCount() > 0) {
                phoneIndex = phones.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER);
            }
            contactsData.telephone = new ArrayList();
            while(phones.moveToNext()) {
                String phoneNumber = phones.getString(phoneIndex);
                contactsData.telephone.add(phoneNumber);
            }
            phones.close();
            /*
             * 查找该联系人的email信息
             */
            Cursor emails = getContentResolver().query(ContactsContract.CommonDataKinds.Email.CONTENT_URI,
                                                       null,
                                                       ContactsContract.CommonDataKinds.Email.CONTACT_ID + "=" + contactId,
                                                       null, null);
            int emailIndex = 0;
            if(emails.getCount() > 0) {
                emailIndex = emails.getColumnIndex(ContactsContract.CommonDataKinds.Email.DATA);
            }
            contactsData.email = new ArrayList();
            while(emails.moveToNext()) {
                String email = emails.getString(emailIndex);
                contactsData.email.add(email);
            }
            emails.close();
            collections.add(contactsData);
            
            
        }
        cursor.close();
        // JSON格式
        //        {
        //            "all_contacts": [
        //            {
        //                "id": "112000",
        //                    "name": "赵四",
        //                    "all_telephone": [
        //                {
        //                    "telephone": "112200"
        //                },
        //                {
        //                    "telephone": "112200"
        //                }
        //                ],
        
        
        //                "all_email": [
        //                {
        //                    "email": "abc@gmail.com"
        //                },
        //                {
        //                    "email": "abc@gmail.com"
        //                }
        //                ]
        //            }
        //            ]
        //        }
        String json = "{\"all_contacts\":[";
        String tmpJson = "";
        String tempJson1 = "";
        String tempJson2 = "";
        Log.i(TAG, "============> collections.size  = " + collections.size());
        int i = 0;
        
        for(ContactsData cd : collections)
        {
            tempJson1 = "";
            tmpJson += "{\"id\":\"" + cd.id + "\"" + ",\"name\":\"" + cd.name + "\", \"all_telephone\":[";
            Log.i(TAG, "i = " + (++i) + ", id = " + cd.id + ", name = " + cd.name);
            for (String telephone : cd.telephone)
            {
                tempJson1 += "{\"telephone\":\"" + telephone + "\"},";
                Log.i(TAG, "telephone = " + telephone);
            }
            tmpJson += tempJson1 + "], \"all_email\": [";
            tempJson1 = "";
            for (String email : cd.email)
            {
                tempJson1 += "{\"email\": \"" + email + "\"},";
                Log.i(TAG, "email = " + email);
            }
            tmpJson += tempJson1 + "]},";
            
        }
        json += tmpJson + "]}";
        Log.i(TAG, "json ===> " + json);
        
    }

	
}
