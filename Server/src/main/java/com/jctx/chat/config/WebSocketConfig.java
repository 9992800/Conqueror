package com.jctx.chat.config;

import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.http.server.ServerHttpRequest;
import org.springframework.http.server.ServerHttpResponse;
import org.springframework.web.socket.WebSocketHandler;
import org.springframework.web.socket.config.annotation.EnableWebSocket;
import org.springframework.web.socket.config.annotation.WebSocketConfigurer;
import org.springframework.web.socket.config.annotation.WebSocketHandlerRegistry;
import org.springframework.web.socket.server.HandshakeInterceptor;

import com.jctx.chat.controller.ChatEndpointHandler;
import com.jctx.chat.controller.MarcoHandler;

@Configuration
@EnableWebSocket
public class WebSocketConfig implements WebSocketConfigurer {
	private static final Logger logger =
		    LoggerFactory.getLogger(WebSocketConfig.class);
	@Override
	public void registerWebSocketHandlers(
	                                 WebSocketHandlerRegistry registry) {
		registry.addHandler(marcoHandler(), "/marco").setAllowedOrigins("http://chat.jucaicun.com");
		registry.addHandler(chateHandler(), "/online").setAllowedOrigins("http://chat.jucaicun.com");
	} 
	
	@Bean
	public HandshakeInterceptor interceptor(){
		return new HandshakeInterceptor(){

			@Override
			public boolean beforeHandshake(ServerHttpRequest request, ServerHttpResponse response,
					WebSocketHandler wsHandler, Map<String, Object> attributes) throws Exception {
				logger.info("---beforeHandshake---"+request.getURI().toString());
				return true;
			}

			@Override
			public void afterHandshake(ServerHttpRequest request, ServerHttpResponse response,
					WebSocketHandler wsHandler, Exception exception) {	
			}
		};
	}
	
	@Bean
	public MarcoHandler marcoHandler() {
	     return new MarcoHandler();
	}
	
	@Bean
	public ChatEndpointHandler chateHandler() {
		return new ChatEndpointHandler();
	}
}