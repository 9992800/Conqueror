package com.jctx.chat.service;

import java.util.ArrayList;

import javax.annotation.Resource;

import org.springframework.stereotype.Service;

import com.jctx.chat.dao.ChatMsgDao;
import com.jctx.chat.dao.ChatUserDao;
import com.jctx.chat.module.MessageBean;
import com.jctx.chat.module.OnlineBean;

@Service
public class ChatDbService {
	
	@Resource ChatUserDao chatDao;
	
	@Resource ChatMsgDao chatMsgDao;

	@Resource ChatMessageCacheForDb msgCache;
	
	public void OnlineUser(OnlineBean bean) {
		
		OnlineBean beanInDb = chatDao.getUserBean(bean.getSelf());
		
		if (null == beanInDb){
			
			chatDao.newOnlineUser(bean);
			
		}else{
			
			chatDao.updateOnlineUser(bean);
		}
	}

	public void saveChatMessageToDb(String fromUid, MessageBean message) {
		
		msgCache.add(message);
		
		if (msgCache.needFlush()){
			
			msgCache.flushMessage();
		}
	}

	public int flushMsgToDb() { 
		return msgCache.flushMessage();
		//return 0;
	}

	public ArrayList<MessageBean> getCurrentQueue() {
		 
		ArrayList<MessageBean> clone = new ArrayList<MessageBean>(msgCache);
		return clone;
	}
}