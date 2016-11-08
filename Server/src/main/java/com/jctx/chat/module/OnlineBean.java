package com.jctx.chat.module;

import java.sql.Date;

public class OnlineBean {
	
	int  device, self;
	String userName, token, sessionId, adminToken;
	Date onlineTime;
	/**
	 * 是admin用户
	 */
	boolean isAdmin;
	/**
	 * 是否接收消息推送
	 */
	Boolean receivePush;
	public Date getOnlineTime() {
		return onlineTime;
	}
	public void setOnlineTime(Date onlineTime) {
		this.onlineTime = onlineTime;
	}
	long system; 
	
	public int getSelf() {
		return self;
	}
	public void setSelf(int self) {
		this.self = self;
	}
	public int getDevice() {
		return device;
	}
	public void setDevice(int device) {
		this.device = device;
	}
	public String getUserName() {
		return userName;
	}
	public void setUserName(String userName) {
		this.userName = userName;
	}
	public String getToken() {
		return token;
	}
	public void setToken(String token) {
		this.token = token;
	}
	public long getSystem() {
		return system;
	}
	public void setSystem(long system) {
		this.system = system;
	}
	public boolean isAdmin() {
		return isAdmin;
	}
	public void setAdmin(boolean isAdmin) {
		this.isAdmin = isAdmin;
	}
	@Override
	public String toString() {
		return "OnlineBean [device=" + device + ", self=" + self + ", userName=" + userName + ", token=" + token
				+ ", sessionId=" + sessionId + ", onlineTime=" + onlineTime + ", system=" + system + "]";
	}
	public void setSessionId(String id) { 
		this.sessionId = id;
	} 
	public String getSessionId(){
		return this.sessionId;
	}
	public String getAdminToken() {
		return adminToken;
	}
	public void setAdminToken(String adminToken) {
		this.adminToken = adminToken;
	}
	public Boolean getReceivePush() {
		return receivePush;
	}
	public void setReceivePush(Boolean receivePush) {
		this.receivePush = receivePush;
	}
}
