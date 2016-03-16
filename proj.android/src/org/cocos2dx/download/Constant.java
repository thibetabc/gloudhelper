package org.cocos2dx.download;
 
/****************************
 * 创建时间：2014年4月1日 下午2:57:56  
 * 项目名称：Gloud  
 * @author 赵强
 * @version 1.0   
 * @since JDK 1.8.0
 * 文件名称：Constant.java  
 * 类说明：  常量
 ****************************/

public interface Constant {
	public final String NEW_CLIENT_URL="http://cdn2.51ias.com/update/gloudlatest.apk";
	
	//public final String DOWN_LOAD_GLOUDCLIENT_URL="http://c4.51ias.com/api.php?m=Anony&a=download&deviceid=%s&pid=%s&client=gloudhelper&target=gloudsingle";
	public final String DOWN_LOAD_GLOUDCLIENT_URL="http://b2.51ias.com/api.php?m=Anony&a=download&deviceid=%s&pid=gloud&client=gloudhelper&target=gloudsingle";
	 
	public final String UPDATE_PROGRESS="progress";
	public final String UPDATE_FAIL="update_fail";
	public final String UPDATE_PROGRESS_ACTION="gloud.update.prgress";
	public final String UPDATE_SERVER_PROGRESS_ACTION="gloud.update.server.progress";
	public final String UPDATE_FRESH_GAME_LIST="gloud.fresh.gamelist";
	public final String SHOW_ERROR="gloud.error.dialog";
	public final String PROGRESS="progress";
	public final String NEW_VERSION="new_version";
	public final String SHOWNAME="show_name";
	public final String URL_STR="url";

	public final String DOWNLOAD_TYPE = "download_type";
	public final String DOWNLOAD_GLOUD_GAME = "download_gloud_game";
	public final String DOWNLOAD_GLOUD_HELPER = "download_gloud_helper";
}