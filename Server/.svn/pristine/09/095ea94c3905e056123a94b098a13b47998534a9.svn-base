package com.jctx.chat.service;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

import com.jctx.chat.module.OnlineUserEntity;

@Component
public class ChatRedisService{
	
	private static final Logger logger =
		    LoggerFactory.getLogger(ChatRedisService.class);	
	
	public ChatRedisService(){
	}

	public void OfflineUser(OnlineUserEntity entity){
		if (null == entity){
			logger.warn("------Offline failed: the user has already offline");
			return;
		}
		entity.offline();
	}
	
	public void OnlineUser(String userId, OnlineUserEntity entity){
		entity.online();
	}

	public int sendMessageToTarget(OnlineUserEntity entity, String targetUid, String payload) {
		if (null == entity){
			logger.warn("------Send message failed: the sender has offline");
			return 0;
		}
		return entity.sendMessage(targetUid, payload);		 
	}

	public void keepalive(OnlineUserEntity entity) {
		if (null == entity){
			logger.warn("------Keep alive action failed: the entity is offline");
			return;
		}
		entity.keepalive();		
	}
	
}
