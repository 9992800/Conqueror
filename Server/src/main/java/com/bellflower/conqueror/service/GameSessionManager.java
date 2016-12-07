package com.bellflower.conqueror.service;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.springframework.stereotype.Service;
import org.springframework.web.socket.WebSocketSession;

@Service
public class GameSessionManager {
	public static final int max_player_in_one_server = 20000;
	private static final Map<String, WebSocketSession> sessionMap = new HashMap<>(max_player_in_one_server);
	
	public void userOnline(String user_id, WebSocketSession session) {
		WebSocketSession old_one = sessionMap.get(user_id);
		if (old_one != null && old_one.isOpen()){
			try {
				old_one.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}		
		sessionMap.put(user_id, session);
	}
}
