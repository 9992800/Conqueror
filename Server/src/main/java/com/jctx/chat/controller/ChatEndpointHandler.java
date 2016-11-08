package com.jctx.chat.controller;

import javax.annotation.Resource;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.PongMessage;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.AbstractWebSocketHandler;

import com.jctx.chat.Utils.ChatRuntimeUtil;
import com.jctx.chat.Utils.SessionUtils;
import com.jctx.chat.module.OnlineBean;
import com.jctx.chat.service.RedisService;

public class ChatEndpointHandler extends AbstractWebSocketHandler{
	
	private static final Logger logger = LoggerFactory.getLogger(ChatEndpointHandler.class);
	private static final String USER_ID_KEY_IN_SESSION = "uid_in_session";
	
	@Resource
	RedisService jedisService;
	 	
	protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
		int sessionId = (int) session.getAttributes().get(USER_ID_KEY_IN_SESSION);
		logger.info("------Received text  message: ("+ message.getPayload()+") sessionId=" + sessionId);
		jedisService.handleTextMessage(session, message.getPayload());
		ChatRuntimeUtil.updateSessionActive(session);
	}
	
	@Override
	public void handleTransportError(WebSocketSession session, Throwable exception) throws Exception {
		logger.error("------Transport text error  message!", exception);
		session.close();
	}
	  
	public void afterConnectionEstablished(WebSocketSession session) throws Exception { 
		OnlineBean bean = new OnlineBean();
		SessionUtils.parseParam(bean, session);
		logger.info("------Established session parameter bean: " + bean.toString());    
		session.getAttributes().put(USER_ID_KEY_IN_SESSION, bean.getSelf());
		jedisService.online(bean, session);
		ChatRuntimeUtil.addSession((long) bean.getSelf(), session);
	}
	  
	public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
		int sessionId = (int) session.getAttributes().get(USER_ID_KEY_IN_SESSION);
		logger.info("------Closed sessionId: " + sessionId +" status = " + status);
		jedisService.onffline(session);
		ChatRuntimeUtil.removeSession((long) sessionId, session);
	}
		
	protected void handlePongMessage(WebSocketSession session, PongMessage message) throws Exception {
		int sessionId = (int) session.getAttributes().get(USER_ID_KEY_IN_SESSION);
		logger.info("------Received pong message: sessionId = " + sessionId);
		jedisService.keepalive(session);
		ChatRuntimeUtil.updateSessionActive(session);
	}
	 
	//--------------------unavailable right now
	protected void handleBinaryMessage(WebSocketSession session, BinaryMessage message) throws Exception {
		logger.info("------Received binary message: " + message.getPayload().toString());
	}
}
