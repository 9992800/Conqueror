package com.jctx.chat.service;

import java.net.Inet4Address;
import java.net.UnknownHostException;
import java.util.HashSet;
import java.util.Set;

import javax.annotation.Resource;
import org.json.JSONObject;
import org.springframework.data.redis.connection.jedis.JedisConnection;
import org.springframework.data.redis.connection.jedis.JedisConnectionFactory;
import org.springframework.stereotype.Service;
import com.jctx.chat.module.JccMsgBean;
import com.jctx.chat.module.MessageBean;

import redis.clients.jedis.Jedis;

@Service
public class JccApiService {
	
	@Resource
	JedisConnectionFactory jedisFactory;

	public Long fromSysToPrivate(JccMsgBean bean) {
		
		JSONObject msg = new JSONObject();
		msg.put("content", bean.getMessage());
		msg.put("channelType", MessageBean.CHANNEL_TYPE_SYSTEM);
		msg.put("from", MessageBean.SYSTEM_CHANNEL);
		msg.put("to", bean.getReceiverUid());
		msg.put("type", bean.getType());
		msg.put("createTime", System.currentTimeMillis()/1000);
		
		JedisConnection conn = null;
		try {
			conn = (JedisConnection) jedisFactory.getConnection();
			Jedis worker = conn.getNativeConnection();
			
			Long reply = worker.publish(bean.getReceiverUid(), msg.toString());
			if (0 == reply){
				return worker.rpush(bean.getReceiverUid(), msg.toString());
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}finally {			
			conn.close();			
		}
		
		return 0L;
	}

	public void broadcast(JccMsgBean bean, boolean isLive) {
		
		JSONObject msg = new JSONObject();
		msg.put("content", bean.getMessage());
		msg.put("channelType", MessageBean.CHANNEL_TYPE_SYSTEM);
		msg.put("from", MessageBean.SYSTEM_CHANNEL);
		msg.put("to", "-1");
		msg.put("type", bean.getType());
		long createTime = System.currentTimeMillis()/1000;
		msg.put("createTime", createTime);
		String message = msg.toString();
		
		JedisConnection conn = null;
		try {
			conn = (JedisConnection) jedisFactory.getConnection();
			Jedis worker = conn.getNativeConnection();
			worker.zadd(MessageBean.SYSTEM_CHANNEL, createTime, message);
			worker.publish(MessageBean.SYSTEM_CHANNEL, message);
			long time_to_delete = createTime - MessageBean.SYSTEM_MSG_TIMEOUT;//;
			worker.zremrangeByScore(MessageBean.SYSTEM_CHANNEL, "-inf", ""+time_to_delete);
		} catch (Exception e) {
			e.printStackTrace();
		}finally {			
			conn.close();			
		}
	}
	
	/**
	 * 将消息发布到redis的admin频道
	 * @param bean
	 */
	public void broadcastToAdmin(JccMsgBean bean) {
		JSONObject msg = new JSONObject();
		msg.put("content", bean.getMessage());
		msg.put("channelType", MessageBean.CHANNEL_TYPE_ADMIN);
		msg.put("from", MessageBean.ADMIN_CHANNEL);
		msg.put("to", "0");
		msg.put("type", bean.getType());
		long createTime = System.currentTimeMillis()/1000;
		msg.put("createTime", createTime);
		String message = msg.toString();
		
		JedisConnection conn = null;
		try {
			conn = (JedisConnection) jedisFactory.getConnection();
			Jedis worker = conn.getNativeConnection();
			worker.publish(MessageBean.ADMIN_CHANNEL, message);
		} catch (Exception e) {
			e.printStackTrace();
		}finally {			
			conn.close();			
		}
	}

	public static void main(String[] args) {
		String ip = "192.168.1.%s";
		Set<Integer> set = new HashSet<>();
		try {
			String addr = Inet4Address.getLocalHost().getHostAddress();
			System.out.println(addr);
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
		for (int i = 0; i <= 254; i ++) {
			String ip0 = String.format(ip, i);
			if (set.contains(ip0.hashCode())) {
				System.out.println(set.size());
			}
			set.add(ip0.hashCode());
		}
	}
}
