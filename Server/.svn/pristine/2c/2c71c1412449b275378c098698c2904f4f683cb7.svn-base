package com.jctx.chat.service;

import java.util.ArrayList;

import javax.annotation.Resource;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.jctx.chat.dao.ChatMsgDao;
import com.jctx.chat.module.MessageBean;

public class ChatMessageCacheForDb extends ArrayList<MessageBean> {
	
	private static final Logger logger =
		    LoggerFactory.getLogger(ChatMessageCacheForDb.class);

	private static final long serialVersionUID = -6408973966469451526L; 
	
	@Resource ChatMsgDao chatMsgDao;
	
	int threshold;
	
	public ChatMessageCacheForDb(int capacity, int threshold){
		super(capacity);
		this.threshold = threshold;
		logger.warn("-----------------capacity="+capacity+"  threshold=" + threshold);
	}

	public ChatMessageCacheForDb(ChatMessageCacheForDb chatMessageCacheForDb) {
		super(chatMessageCacheForDb);
	}

	public boolean needFlush() { 
		return this.size() >= this.threshold;
	}

	public int flushMessage() {
		ChatMessageCacheForDb tmp = new ChatMessageCacheForDb(this);
		logger.warn("------message cache start to swap data.-------");
		this.clear();
		
		FlushTask task = new FlushTask(tmp);
		task.start();
		return tmp.size();
	}
	
	class FlushTask extends Thread{
		
		ChatMessageCacheForDb messageQueue;
		
		public FlushTask(ChatMessageCacheForDb queue){
			this.messageQueue = queue;
		}
		
		@Override
		public void run(){			
			chatMsgDao.saveMessageList(this.messageQueue);
		}
	};
	
	
}
