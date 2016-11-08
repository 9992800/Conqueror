package com.jctx.chat.module;

import java.io.IOException;

import org.json.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.data.redis.connection.jedis.JedisConnectionFactory;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;

import com.jctx.chat.Utils.ChatRuntimeUtil;

import redis.clients.jedis.JedisPubSub;

public class OnlineUserEntity  extends JedisPubSub{
	
	private static final Logger logger = LoggerFactory.getLogger(OnlineUserEntity.class);
	
	MessageReaderPipe msgPuller;
	
	MessageWriterPipe msgPublisher;
	
	OnlineBean bean;
	WebSocketSession session;

	private long updateTime;
	
	public OnlineUserEntity(JedisConnectionFactory jedisFactory, OnlineBean bean, WebSocketSession session) {
		
		this.bean = bean;
		
		this.session = session;

		msgPuller = new MessageReaderPipe(jedisFactory, this, bean, session);
		
		msgPublisher = new MessageWriterPipe(jedisFactory, bean);
		
		updateTime = System.currentTimeMillis();
	}

	public void online() {
		msgPuller.start();	
	}

	public void offline() {
		try {
			msgPuller.close();
			msgPublisher.close(); 
		} catch (Exception e) {
			e.printStackTrace();
		}
	} 
	
	@Override
	public void onMessage(String channel, String message) {
		logger.info("---redis get message---channel="+channel+" message="+message);
		try {
			JSONObject msgobj = new JSONObject();
			msgobj.put("content", message);
			msgobj.put("type", MessageBean.MESSAGE_TYPE_LIVE);
			this.session.sendMessage(new TextMessage(msgobj.toString()));
			ChatRuntimeUtil.updateSessionActive(session);
		} catch (IOException e) { 
			e.printStackTrace();
		}
	}
	
	public void onSubscribe(String channel, int subscribedChannels) {
		logger.info("---subscribe success---channel:"+channel+" subscribedChannels=" + subscribedChannels);   
	}
	
	public void onUnsubscribe(String channel, int subscribedChannels) {
		logger.info("---unsubscribe success---channel:"+channel+" subscribedChannels=" + subscribedChannels);
	} 
	
	/**
	 * @param targetUid
	 * @param payload
	 * @return 目标用户未读的消息数量
	 */
	public int sendMessage(String targetUid, String payload) {
		return msgPublisher.pushlishToTarget(targetUid, payload);		
	}

	public void keepalive() {
		this.updateTime = System.currentTimeMillis();		
	}
	
	public long getUpdateTime(){
		return this.updateTime;
	}

	public OnlineBean getBean() {
		return bean;
	}

	public void setBean(OnlineBean bean) {
		this.bean = bean;
	}
}
