package org.cocos2dx.download;


import android.annotation.SuppressLint;
import android.content.Context;
import android.content.pm.PackageManager.NameNotFoundException;
import java.text.SimpleDateFormat;
import java.util.Date;
 

@SuppressLint("SimpleDateFormat")
public class StringUtils 
{
    /**
    * >Checks if a CharSequence is not empty ("") and not null.
    *
    * >
    * StringUtils.isNotEmpty(null)      = false
    * StringUtils.isNotEmpty("")        = false
    * StringUtils.isNotEmpty(" ")       = true
    * StringUtils.isNotEmpty("bob")     = true
    * StringUtils.isNotEmpty("  bob  ") = true
    * 
    *
    */
   public static boolean isNotEmpty(String cs) {
   	if (null==cs) {
			return false;
		}else if("".equals(cs)) {
			return false;					
		}
   	return true;
   }
	
	
	 /**
     * >Checks if a CharSequence is not empty (""), not null and not whitespace only.</p>
     *
     * >
     * StringUtils.isNotBlank(null)      = false
     * StringUtils.isNotBlank("")        = false
     * StringUtils.isNotBlank(" ")       = false
     * StringUtils.isNotBlank("bob")     = true
     * StringUtils.isNotBlank("  bob  ") = true
     * </
     *
     */
    public static boolean isNotBlank(String cs) {
        return !StringUtils.isBlank(cs);
    }
    
    
    
    
    
    /**
     * >Checks if a CharSequence is whitespace, empty ("") or null.</p>
     *
     * >
     * StringUtils.isBlank(null)      = true
     * StringUtils.isBlank("")        = true
     * StringUtils.isBlank(" ")       = true
     * StringUtils.isBlank("bob")     = false
     * StringUtils.isBlank("  bob  ") = false
     * </
     *
     * @param cs  the CharSequence to check, may be null
     * @return {@code true} if the CharSequence is null, empty or whitespace
     * @since 2.0
     * @since 3.0 Changed signature from isBlank(String) to isBlank(CharSequence)
     */
    public static boolean isBlank(String cs) {
        if (null==cs) {
			return true;
		}else  {
			cs=cs.replaceAll(" ", "");
			if ("".equals(cs)) {
				return true;
			}
		}
        return false;
    }
    
    
      /**
     * >Checks if a CharSequence is empty ("") or null.</p>
     *
     * >
     * StringUtils.isEmpty(null)      = true
     * StringUtils.isEmpty("")        = true
     * StringUtils.isEmpty(" ")       = false
     * StringUtils.isEmpty("bob")     = false
     * StringUtils.isEmpty("  bob  ") = false
     * </
     *
     * >NOTE: This method changed in Lang version 2.0.
     * It no longer trims the CharSequence.
     * That functionality is available in isBlank().</p>
     *
     */
    public static boolean isEmpty(String cs) {
        if (null==cs) {
			return true;
		}else if ("".equals(cs)) {
			return true;
		}
        return false;
    }
    
 // Trim
    //-----------------------------------------------------------------------
    /**
     * >Removes control characters (char &lt;= 32) from both
     * ends of this String, handling {@code null} by returning
     * {@code null}.</p>
     *
     * >The String is trimmed using {@link String#trim()}.
     * Trim removes start and end characters &lt;= 32.
     * To strip whitespace use {@link #strip(String)}.</p>
     *
     * >To trim your choice of characters, use the
     * {@link #strip(String, String)} methods.</p>
     *
     * >
     * StringUtils.trim(null)          = null
     * StringUtils.trim("")            = ""
     * StringUtils.trim("     ")       = ""
     * StringUtils.trim("abc")         = "abc"
     * StringUtils.trim("    abc    ") = "abc"
     * </
     *
     */
    public static String trim(String str) {
        return str == null ? null : str.trim();
    }
        
    
    public static String longTiimeCoverString(long time)
    {
    	SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd HH:mm");
    	Date date=new Date(time);
    	return sdf.format(date);
    }
    
    public static String getCacheDirPath(Context ctx)
    {
    	try {
			if (ExistSDCard()) {
				return ctx.getExternalCacheDir().getAbsolutePath();
			}else {
				return ctx.getCacheDir().getAbsolutePath();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
    	return ctx.getCacheDir().getAbsolutePath();
    }
    
    
    private static boolean ExistSDCard() {
  	  if (android.os.Environment.getExternalStorageState().equals(
  	    android.os.Environment.MEDIA_MOUNTED)) {
  	   return true;
  	  } else
  	   return false;
  	 }
    
	/**
	 * get versionCode
	 * 
	 * @param context
	 * @return
	 */
	public static int getVersionCode(Context context) {
		int versionCode = -1;
		try {
			versionCode = context.getPackageManager().getPackageInfo(
					context.getPackageName(), 0).versionCode;
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return versionCode;
	}

	/**
	 * get versionName
	 * 
	 * @param context
	 * @return
	 */
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
}
  
