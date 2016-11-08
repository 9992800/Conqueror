package com.jctx.chat.dao;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.springframework.stereotype.Repository;

import com.jctx.chat.module.ChatUserBean;
import com.jctx.chat.module.MessageBean;
import com.jctx.chat.service.ChatMessageCacheForDb;

@Repository
public class ChatMsgDao extends  ChatDao {
	static final String NAME_SPACE = "com.jctx.chat.message";
	public void saveMessageList(ChatMessageCacheForDb messageQueue) {
		
		this.getSqlSession().insert(NAME_SPACE+".record", messageQueue);	
	}
	public List<MessageBean> getHistoryMessage(ChatUserBean bean, int peerUid, int startMsgId) {
		HashMap<String, Object> params = new HashMap<>();
		
		params.put("from", bean.getUid());
		params.put("to", peerUid);
		params.put("msgId", startMsgId);
		
		List<MessageBean> list =  this.getSqlSession().selectList(NAME_SPACE+".historyMsg", params);
		if (null == list){
			list = new ArrayList<>();
		}
		
		return list;
	}

}
