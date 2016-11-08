package com.jctx.chat.module;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.json.JSONArray;
import org.json.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.data.redis.connection.RedisConnection;
import org.springframework.data.redis.connection.jedis.JedisConnection;
import org.springframework.data.redis.connection.jedis.JedisConnectionFactory;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPubSub;


public class MessageReaderPipe extends Thread{
	
	private static final Logger logger =
		    LoggerFactory.getLogger(MessageReaderPipe.class);
	RedisConnection subconn;
	
	Jedis subscriber;
	
	JedisConnectionFactory jedisFactory;
	
	private JedisPubSub pubsub;
	
	OnlineBean bean;
	private WebSocketSession session;
	
	public MessageReaderPipe(JedisConnectionFactory jedisFactory,
			JedisPubSub pubsub, OnlineBean bean, WebSocketSession session) {
		
		this.jedisFactory = jedisFactory;
		this.bean = bean;		
		this.pubsub = pubsub;
		this.session = session;
		
		subconn =  jedisFactory.getConnection();		
		subscriber = ((JedisConnection)subconn).getNativeConnection();
		subscriber.clientSetname("sub:" +bean.getSelf());
	}

	private void sendToClient(WebSocketSession session, List<String> unread_messages) throws IOException {
		if (unread_messages.size() == 0){
			return ;
		}
		
		JSONArray jsonMsgs = new JSONArray(unread_messages);
		
		JSONObject msgobj = new JSONObject();
		msgobj.put("content", jsonMsgs);
		msgobj.put("type", MessageBean.MESSAGE_TYPE_UNREAD);
		
		String msgs = msgobj.toString();
		
		session.sendMessage(new TextMessage(msgs));
	}

	private List<String> getUnreadMessage(OnlineBean bean) {
		
		List<String> unread_messages = new ArrayList<>();
		unread_messages = subscriber.lrange(""+bean.getSelf(), 0, -1);
		
		subscriber.del(""+bean.getSelf());
		
		Set<String> system_messages = subscriber.zrangeByScore(MessageBean.SYSTEM_CHANNEL,
				"("+bean.getSystem(), ""+System.currentTimeMillis()/1000);
		
		unread_messages.addAll(system_messages);
		return unread_messages;
	}

	public void close() {	
		if (this.pubsub.isSubscribed()){
			this.pubsub.unsubscribe(); 
		}
	}

	@Override
	public void run() {
		try{			
			if (bean.isAdmin()) {//是运营管理员
				subscriber.subscribe(this.pubsub, String.valueOf(bean.getSelf()), MessageBean.ADMIN_CHANNEL);
			} else {//是普通用户
				List<String> unread_messages = this.getUnreadMessage(bean);
				this.sendToClient(this.session, unread_messages);
				subscriber.subscribe(this.pubsub, String.valueOf(bean.getSelf()), MessageBean.SYSTEM_CHANNEL);
			}
			subscriber.clientSetname("");
			logger.info("Tread id:"+this.getId()+" destroyed");
		}catch(Exception e){
			logger.error("MessageReaderPipe thread error!", e);
		}finally {
			subconn.close();			
		}
	}
}
