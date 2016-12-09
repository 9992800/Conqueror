package com.bellflower.conqueror.controller;

import javax.annotation.Resource;

import org.json.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.PongMessage;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.AbstractWebSocketHandler;

import com.bellflower.conqueror.Utils.SessionUtils;
import com.bellflower.conqueror.module.OnlineBean;
import com.bellflower.conqueror.service.GameDispatchService;

public class FindComponetProxy extends AbstractWebSocketHandler {
	private static final Logger logger =
	    LoggerFactory.getLogger(FindComponetProxy.class);
	  @Resource GameDispatchService gameDispatchService;
	  
	 protected void handleTextMessage(
	     WebSocketSession session, TextMessage message) throws Exception {
	  
		 logger.info("Received text  message: " + message.getPayload());	     
		 session.sendMessage(new TextMessage("......哈喽..Polo!......."));
	  }
	  
	public void afterConnectionEstablished(WebSocketSession session) throws Exception {
		 OnlineBean bean = new OnlineBean();		
		 SessionUtils.parseParam(bean, session); 
		 logger.info("user online: " + bean.toString());  
		 
		 JSONObject resut = gameDispatchService.findOppoent(bean, session);
		 resut.put("msg_type", MESSGAE_TYPE_CONSTS.ONLINE_MESSAGE_RESPONSE_TYPE_FIND_COMPONENT.getType());
		 
		 session.sendMessage(new TextMessage(resut.toString()));	
	}
	  
	public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
		 logger.info("session getId: " + session.getId()+" status ="+status);

		 gameDispatchService.userOffline(session);
	}
	
	protected void handleBinaryMessage(WebSocketSession session, BinaryMessage message) throws Exception {
		logger.info("Received binary message: " + message.getPayload().toString());
	}

	protected void handlePongMessage(WebSocketSession session, PongMessage message) throws Exception {
		logger.info("Received pong message: " + message.getPayload().toString());
	}
}  
