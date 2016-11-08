package com.jctx.chat.module;

import org.springframework.data.redis.connection.jedis.JedisConnection;
import org.springframework.data.redis.connection.jedis.JedisConnectionFactory;

import redis.clients.jedis.Jedis;

public class MessageWriterPipe  implements Runnable{
	 
	JedisConnectionFactory jedisFactory;

	private OnlineBean bean;
	
	public MessageWriterPipe(JedisConnectionFactory jedisFactory, OnlineBean bean){
		this.jedisFactory = jedisFactory;
		this.bean = bean;
	}

	public void close() {
	}

	public int pushlishToTarget(String targetUid, String payload) {
		Jedis jedis = ((JedisConnection)jedisFactory.getConnection()).getNativeConnection();
		try {
			boolean offline = jedis.publish(targetUid, payload) == 0;
			if (offline){
				jedis.rpush(targetUid, payload);
				return jedis.llen(targetUid).intValue();
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			jedis.close();
		}
		return 0;
	}

	@Override
	public void run() {
	}
}
