package org.cocos2dx.download;

import java.io.File;
import java.util.Locale;

import net.tsz.afinal.FinalHttp;
import net.tsz.afinal.http.AjaxCallBack;
import net.tsz.afinal.http.HttpHandler;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;
 
/****************************
 * 创建时间：2014年4月8日 下午4:41:02  
 * 项目名称：Gloud  
 * @author 赵强
 * @version 1.0   
 * @since JDK 1.8.0
 * 文件名称：UpdateUtils.java  
 * 类说明：  升级工具类
 ****************************/

public class UpdateUtils implements Constant{
	
	//private GloudDialog mUpdateDialog;
	private Context mContext;
	private static String mApkPath;
	//private static NetDataTask mNetDataTask;
	private FinalHttp mFinalHttp;
	private HttpHandler<File> mHandler;
	public UpdateUtils(final Context ctx,final Iprogress progressInterface)
	{
		mHandler = null;
		mContext=ctx;
		//mSharedPrefUtil=new SharedPrefUtil(mContext, VERSION_DATA);
	}


//	public void Update(final boolean isMain)
//	{
//		if (null!=mNetDataTask&&mNetDataTask.getStatus()==Status.RUNNING) {
//			return;
//		}
//		AjaxParams params=new AjaxParams();
//		params.put(PID, GeneralUtils.getChannel(mContext));
//		params.put(VER, StringUtils.getVersionCode(mContext)+"");
//		params.put(A, CLIENT_VERSION);
//		params.put(M, ANONY);
//		
//		mNetDataTask=new NetDataTask(mContext, ConStantUrl.getInstance(mContext).URL_BASE(), params, !isMain,mContext.getString(R.string.loading), new NetDataTask.DataResult() {
//			@Override
//			public void onDataResult(String result) {
//				// TODO Auto-generated method stub
//				if (null==result||"null".equals(result)||"".equals(result)) {
//					mIprogress.onFail();
//					return;
//				}
//				try {
//					MyLog.z("更新返回值 ==>"+result);
//					org.json.JSONObject jsonObject=new org.json.JSONObject(result);
//					int ret=jsonObject.getInt("ret");
//					if (0==ret) {
//						JSONObject obj=jsonObject.getJSONObject("ver");
//						JSONObject currentObj=obj.getJSONObject("current");
//						JSONObject latestObj=obj.getJSONObject("latest");
//						mLastVersionName=latestObj.getString("name");
//						int ver=latestObj.getInt("ver");
//						String desc=latestObj.getString("desc").toString();
//						final String url=latestObj.getString("url");
//						
//						mSharedPrefUtil.putString(ver+"", desc);
//						mSharedPrefUtil.putString(APP_URL, url);
//						
//						mSharedPrefUtil.putString(StringUtils.getVersionCode(mContext)+"",
//								currentObj.getString("desc"));
//						
//						mSharedPrefUtil.commit();
//						
//						if (ver<=StringUtils.getVersionCode(mContext)) {
//							if (!isMain) {
//								GloudToast.makeText(mContext, R.string.no_update, 1).show();
//							}
//							mIprogress.onContinue();
//							return;
//						}
//						if (null==mUpdateDialog) {
//							mUpdateDialog=new GloudDialog(mContext);
//							mUpdateDialog.BulidUpdate(desc, new OnClickListener() {
//								
//								@Override
//								public void onClick(View arg0) {
//										String filename=url.substring(url.lastIndexOf("/"));
//										mApkPath=StringUtils.getCacheDirPath(mContext)+filename;
//										Intent toUpdate=new Intent(mContext,UpdateActivity.class);
//										toUpdate.putExtra(NEW_VERSION, mLastVersionName);
//										mContext.startActivity(toUpdate);
//										((Activity)mContext).finish();
//										DownloadNewApk(url);
//									mUpdateDialog.dismiss();
//								}
//
//								
//							}, mContext.getString(R.string.now_update), 
//							new OnClickListener() {
//								
//								@Override
//								public void onClick(View arg0) {
//									mUpdateDialog.dismiss();
//								}
//							}, mContext.getString(R.string.next_update));
//							mUpdateDialog.setOnDismissListener(new OnDismissListener() {
//								@Override
//								public void onDismiss(DialogInterface arg0) {
//									if (!((Activity)mContext).isFinishing()) {
//										mIprogress.onContinue();
//									}
//								}
//							});
//							mUpdateDialog.setOnKeyListener(new DialogKeyListener());
//						}
//						mUpdateDialog.show();
//					}
//					else {
//						//更新如果有错误，那么弹错误提示
////						mIprogress.onContinue();
//						mIprogress.onFail(jsonObject.getString("msg"),ret);
//					}
//					
//				} catch (Exception e) {
//					mIprogress.onContinue();
//					e.printStackTrace();
//				}
//				
//			}
//		});
//		mNetDataTask.execute();
//		
//	}
	
	/**
	 * 下载APK 
	 * @param url
	 * **/
	public void downApk(String url)
	{
		downApk(url,"");
	}
	
	/**
	 * 下载APK 
	 * @param url
	 * **/
	public void downApk(String url,String showVersio)
	{
		String filename=url.substring(url.lastIndexOf("/"));
		mApkPath=StringUtils.getCacheDirPath(mContext)+filename;
		DownloadNewApk(url);
	}
	
	public interface Iprogress{
		public void onLoading(long count,long current);
		public void onContinue();			//更新走完接着走下一步
		public void onFail();
		public void onFail(String msg,int ret);
		
	}
	
	
	public void installLocationApk()
	{
		Intent intent = new Intent();
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.setAction(android.content.Intent.ACTION_VIEW);
        intent.setDataAndType(Uri.fromFile(new File(mApkPath)),
                        "application/vnd.android.package-archive");
        mContext.startActivity(intent);
	}

	
	/**
	 * 下载新APK
	 * */
	public void DownloadNewApk(final String url) {
		if (null == mFinalHttp) {
			String filename = url.substring(url.lastIndexOf("/")).toLowerCase(
					Locale.getDefault());
			if (!filename.contains("apk")) {
				filename = "gloudsingle.apk";
			}
			mApkPath = StringUtils.getCacheDirPath(mContext) + "/" + filename;
			Log.e("ZQ", "url+=" + url + "--------->" + mApkPath);
			mFinalHttp = new FinalHttp();
			mHandler = mFinalHttp.download(url, mApkPath,
					new AjaxCallBack<File>() {

						@Override
						public boolean isProgress() {
							return super.isProgress();
						}

						@Override
						public void onFailure(Throwable t, int errorNo,
								String strMsg) {
							super.onFailure(t, errorNo, strMsg);
							mFinalHttp = null;
							Intent intent = new Intent(UPDATE_PROGRESS_ACTION);
							intent.putExtra(UPDATE_FAIL, true);
							mContext.sendBroadcast(intent);
							Log.e("ZQ", url + "下载失败....." + strMsg);
						}

						@Override
						public void onLoading(long count, long current) {
							Intent intent = new Intent(UPDATE_PROGRESS_ACTION);
							intent.putExtra(UPDATE_FAIL, false);
							float pross = (current / (float) count) * 100;
							intent.putExtra(UPDATE_PROGRESS, pross);
							mContext.sendBroadcast(intent);
							Log.e("onLoading()", "进度======》" + pross);
							super.onLoading(count, current);

						}

						@Override
						public void onStart() {
							Log.e("onStart", "onStart() called");
							super.onStart();
						}

						@Override
						public void onSuccess(File t) {
							mFinalHttp = null;
							Log.e("ZQ", "onSuccess....." + t.getAbsolutePath());
							String cmd = "chmod 755 " + t.getAbsolutePath(); /*
																			 * 755
																			 * 权限是对apk自身应用具有所有权限
																			 * ，
																			 * 对组和其他用户具有读和执行权限
																			 */
							try {
								Runtime.getRuntime().exec(cmd);
							} catch (RuntimeException e) {
							} catch (Exception e) {
								e.printStackTrace();
							}
							Intent intent = new Intent();
							intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
							intent.setAction(android.content.Intent.ACTION_VIEW);
							intent.setDataAndType(Uri.fromFile(t),
									"application/vnd.android.package-archive");
							mContext.startActivity(intent);
							super.onSuccess(t);
						}

						@Override
						public AjaxCallBack<File> progress(boolean progress,
								int rate) {
							// TODO Auto-generated method stub
							return super.progress(progress, rate);
						}

					});
		}

	}
	
	public void stopDownload()
	{
		if(null != mHandler){
			mHandler.stop();
		}
	}
}
  
