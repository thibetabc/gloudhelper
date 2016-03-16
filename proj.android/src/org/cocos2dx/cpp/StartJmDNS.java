package org.cocos2dx.cpp;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import javax.jmdns.JmDNS;
import javax.jmdns.ServiceInfo;
import org.apache.http.conn.util.InetAddressUtils;

import android.util.Log;

public class StartJmDNS {

	public static native void addMsgJmDNS(String tInfo);
	public static native void createSceneJNI();
	public static native boolean JniIsStopSearch();
	private final int MAX_TIME = 3;
	public void start() {
		String sType = "_gloudprivateprotocol._tcp.local.";
		try {
			InetAddress tAddr = getLocalAddress();
			Log.i("StartJmDNS", "Local Address:" + tAddr);
			if (tAddr != null) {				
				for(int i=0;i<MAX_TIME;i++)
				{
					Log.i("StartJmDNS", "JmDNS Create");
					JmDNS tJmDNS = JmDNS.create(tAddr, sType);
					Log.i("StartJmDNS", "JmDNS List");
					final ServiceInfo[] tInfos = tJmDNS.list(sType);
					Log.i("StartJmDNS", "JmDNS Find " + tInfos.length + " Apps");
					for (ServiceInfo tInfo : tInfos) {
						Log.i("Start JmDNS", "info :" + tInfo.getName());
						addMsgJmDNS(tInfo.getName());

					}
					Log.i("StartJmDNS", "JmDNS Close");
					tJmDNS.close();
					Log.i("StartJmDNS", "JmDNS Close Complete");
					if(tInfos.length > 0)
					{
						break;					
					}
					if(JniIsStopSearch())
					{
						return;
					}
				}
			}
			createSceneJNI();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	// 获取本地IP
	public static final InetAddress getLocalAddress() {
		try {
			for (Enumeration<NetworkInterface> ei = NetworkInterface
					.getNetworkInterfaces(); ei.hasMoreElements();) {
				NetworkInterface tNet = ei.nextElement();
				if (AddressCanReachable(tNet)) {
					for (Enumeration<InetAddress> tAddr = tNet
							.getInetAddresses(); tAddr.hasMoreElements();) {
						InetAddress tInetAddress = tAddr.nextElement();
						if (!tInetAddress.isLoopbackAddress()
								&& InetAddressUtils.isIPv4Address(tInetAddress
										.getHostAddress())) {
							return tInetAddress;
						}
					}
				}
			}
		} catch (SocketException e) {
			e.printStackTrace();
		}
		return null;
	}

	// 检测网卡有效性
	public static boolean AddressCanReachable(NetworkInterface tNet) {
		MulticastSocket socket = null;
		try {
			socket = new MulticastSocket(5353);
			InetAddress group = InetAddress.getByName("224.0.0.1");
			if(null == group)
			{
				return false;
			}
			socket.setNetworkInterface(tNet);
			socket.joinGroup(group);
			byte[] buffer = new byte[] { 0 };
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length,
					group, 5353);
			socket.send(packet);
			socket.leaveGroup(group);
			return true;
		} catch (IOException e) {
			e.getMessage();
		} finally {
			socket.close();
		}
		return false;
	}
}
