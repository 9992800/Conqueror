package com.jctx.chat.config;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.PropertySource;
import org.springframework.data.mongodb.MongoDbFactory;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.SimpleMongoDbFactory;

import com.mongodb.MongoClientOptions;
import com.mongodb.MongoClientURI;

@Configuration
@PropertySource("classpath:system.properties")
public class MongoDBConfig {
	//mongo.uri=mongodb://userName:passWord@127.0.0.1:27017/DBname
	@Value("${mongodb.uri.str}")
	private String uri;
	public
    @Bean
    MongoDbFactory mongoDbFactory() throws Exception {
        // mongo连接池的参数
        MongoClientOptions.Builder mongoClientOptions =
            MongoClientOptions.builder().socketTimeout(3000).connectTimeout(3000)
                .connectionsPerHost(20);
        // 设置连接池
        MongoClientURI mongoClientURI = new MongoClientURI(uri, mongoClientOptions);
        return new SimpleMongoDbFactory(mongoClientURI);
    }

    public
    @Bean
    MongoTemplate mongoTemplate() throws Exception {
        return new MongoTemplate(mongoDbFactory());
    }
}
