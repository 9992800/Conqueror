package com.jctx.chat.service;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import javax.annotation.Resource;

import org.springframework.data.redis.connection.jedis.JedisConnection;
import org.springframework.data.redis.connection.jedis.JedisConnectionFactory;
import org.springframework.stereotype.Service;

import com.jctx.chat.dao.UserManagerDao;
import com.jctx.chat.module.ChatUserBean;

import redis.clients.jedis.Jedis;

@Service
public class UserManagerService {

	@Resource UserManagerDao userManagerDao;
	@Resource JedisConnectionFactory jedisFactory;
	
	public ChatUserBean getChatUserInfo(int uid) { 
		return userManagerDao.getChatUserInfo(uid);
	}

	public void createChatUser(ChatUserBean bean) {
		userManagerDao.createChatUser(bean);
	}

	public void updateChatUser(ChatUserBean bean) {
		HashMap<String, Object> params = new HashMap<>();
		params.put("uid", bean.getUid());
		if (null != bean.getUserName()){
			params.put("userName", bean.getUserName());
		}
		
		userManagerDao.updateChatUser(params);
	}

	public List<ChatUserBean> getChatUsers(int startUid) { 
		return userManagerDao.getChatUsers(startUid);
	}

	public List<String> getOnlineChatUsers() {
		JedisConnection conn =  (JedisConnection) jedisFactory.getConnection();	
		try { 	
			Jedis redis = conn.getNativeConnection();
			return redis.pubsubChannels(null);
		}catch(Exception e){			
			e.printStackTrace();			
		} finally {
			conn.close();
		}
		return new ArrayList<>();
	}

}
