package com.jctx.chat.service;

import org.springframework.web.socket.WebSocketSession;

import com.jctx.chat.module.OnlineBean;

public interface RedisService {

	void online(OnlineBean bean, WebSocketSession session); 

	void handleTextMessage(WebSocketSession session, String payload);

	void onffline(WebSocketSession session);

	void keepalive(WebSocketSession session);

}
