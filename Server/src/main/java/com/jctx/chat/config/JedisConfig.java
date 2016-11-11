package com.jctx.chat.config;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.PropertySource;
import org.springframework.data.redis.connection.RedisNode;
import org.springframework.data.redis.connection.RedisSentinelConfiguration;
import org.springframework.data.redis.connection.jedis.JedisConnectionFactory;

import redis.clients.jedis.JedisPoolConfig;

//@Configuration 
@PropertySource("classpath:system.properties")
public class JedisConfig {
	private static final Logger logger =
		    LoggerFactory.getLogger(JedisConfig.class);
	
	static final Set<String> sentinels = new HashSet<>();
	 
	
	@Value("${sentinel.ports.property}")
	private List<Integer> sentinelPorts;

	@Value("${sentinel.hosts.property}")
	private List<String> sentinelHosts;
	
	
	@Value("${sentinel.master.name}")
	private String sentinelMaster;
	
	@Value("${redis.pool.maxtotal}")
	private int maxTotal;
	
	@Value("${redis.pool.maxIdle}")
	private int maxIdle;
	
	@Value("${redis.pool.maxWait}")
	private int maxWait; 
	
	@Value("${msg.cache.capacity}")
	private int CAPACITY;
	
	@Value("${msg.cache.threshold}")
	private  int FLUSH_THRESHOLD;
//	
//	@Bean
//	public ChatMessageCacheForDb messageCache(){
//		return new ChatMessageCacheForDb(CAPACITY, FLUSH_THRESHOLD);
//	}
	
	@Bean
	public JedisConnectionFactory jedisConnectionFactory() {
		
		return new JedisConnectionFactory(sentinelConfig(), PoolConfig());	  
	} 
	
	JedisPoolConfig PoolConfig(){ 
		
		JedisPoolConfig config = new JedisPoolConfig();
		
		config.setMaxTotal(maxTotal);
		config.setMaxIdle(maxIdle);
		config.setMaxWaitMillis(maxWait);
		config.setTestOnBorrow(true);
		config.setTestOnReturn(true);
		logger.warn("-----------------maxTotal="+maxTotal+"  maxIdle=" + maxIdle+"  maxWait=" + maxWait);
		return config;
	}
	 
	RedisSentinelConfiguration sentinelConfig() {
		RedisSentinelConfiguration cfg = new RedisSentinelConfiguration();
		
		cfg.master(sentinelMaster);
		
		int index = 0;
		for (String host:sentinelHosts){
			
			int port = sentinelPorts.get(index);			
			RedisNode sentinel = new RedisNode(host, port);			
			cfg.addSentinel(sentinel);
			sentinels.add(host+":"+port);
			index++;			
			logger.warn("-----------------host="+host+"  port=" + port);
		}
		
		return cfg;
	}
}
