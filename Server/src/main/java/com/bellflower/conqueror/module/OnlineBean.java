package com.bellflower.conqueror.module;

import org.springframework.web.socket.WebSocketSession;

public class OnlineBean {
 

	String userId;
	String sessionId;
	String mapId;
	private WebSocketSession session;

	public String getSessionId() {
		return sessionId;
	}

	public void setSessionId(String sessionId) {
		this.sessionId = sessionId;
	}

	public String getUserId() {
		return userId;
	}

	public void setUserId(String userId) {
		this.userId = userId;
	}

	public void setSession(WebSocketSession session) { 
		this.session = session;
	}
	
	public WebSocketSession getSession(){
		return this.session;
	}

	public String getMapId() {
		return mapId;
	}

	public void setMapId(String mapId) {
		this.mapId = mapId;
	}

	@Override
	public String toString() {
		return "OnlineBean [userId=" + userId + ", sessionId=" + sessionId + ", mapId=" + mapId + ", session=" + session
				+ "]";
	}
}
