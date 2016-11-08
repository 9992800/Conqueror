package com.jctx.chat.controller;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.PingMessage;
import org.springframework.web.socket.PongMessage;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.AbstractWebSocketHandler;

public class MarcoHandler extends AbstractWebSocketHandler {
	private static final Logger logger =
	    LoggerFactory.getLogger(MarcoHandler.class);
	  
	 protected void handleTextMessage(
	     WebSocketSession session, TextMessage message) throws Exception {
	  
		 logger.info("Received text  message: " + message.getPayload());
	     
	    session.sendMessage(new TextMessage("Polo!"));
	  }
	  
	public void afterConnectionEstablished(WebSocketSession session) throws Exception {
		 logger.info("session getId: " + session.getId()); 
		 session.sendMessage(new PingMessage());
	}
	  
	public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
		 logger.info("session getId: " + session.getId()+" status ="+status);
	}
	
	protected void handleBinaryMessage(WebSocketSession session, BinaryMessage message) throws Exception {
		logger.info("Received binary message: " + message.getPayload().toString());
	}

	protected void handlePongMessage(WebSocketSession session, PongMessage message) throws Exception {
		logger.info("Received pong message: " + message.getPayload().toString());
	}
}  
