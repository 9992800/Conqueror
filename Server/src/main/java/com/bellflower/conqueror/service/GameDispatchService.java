package com.bellflower.conqueror.service;

import java.util.concurrent.ConcurrentHashMap;

import javax.annotation.Resource;

import org.springframework.stereotype.Service;
import org.springframework.web.socket.WebSocketSession;

import com.bellflower.conqueror.module.OnlineBean;

@Service
public class GameDispatchService {
	
	@Resource GameRoomManager roomManager; 
	 
	public static final int finding_status_wait = 1;
	public static final int finding_status_create = 2; 
	
	//TODO:: 20000 should be replaced by config
	public static final int max_player_in_one_server = 20000;
	static final ConcurrentHashMap<String, WebSocketSession> sessionCache = 
			new ConcurrentHashMap<>(max_player_in_one_server);
	
	static final ConcurrentHashMap<String, OnlineBean> olineUsersCache = 
			new ConcurrentHashMap<>(max_player_in_one_server);
	
	
	
	public void userOnline(WebSocketSession session, OnlineBean bean) throws InterruptedException {
		bean.setSession(session);
		session.getAttributes().put("user_id", bean.getUserId());
		sessionCache.put(session.getId(), session); 
		olineUsersCache.put(bean.getUserId(), bean);
		roomManager.enterRoom(bean);
	}

	public void userOffline(WebSocketSession session) throws InterruptedException { 
		String user_id = (String) session.getAttributes().get("user_id");

		OnlineBean bean = olineUsersCache.get(user_id);
		roomManager.leaveRoom(bean);
		olineUsersCache.remove(user_id);
		sessionCache.remove(session.getId());
	}
	
	public void findOpponent(OnlineBean me, WebSocketSession session) throws InterruptedException {
		this.userOnline(session, me); 	
	}

	public int getOnlineUsers() { 
		return sessionCache.size();
	}  
}
