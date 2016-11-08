package com.jctx.chat.service;

import java.io.IOException;
import java.util.UUID;

import javax.annotation.Resource;

import org.springframework.data.redis.connection.jedis.JedisConnectionFactory;
import org.springframework.stereotype.Service;
import org.springframework.web.socket.WebSocketSession;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.jctx.chat.dao.ChatUserDao;
import com.jctx.chat.module.MessageBean;
import com.jctx.chat.module.OnlineBean;
import com.jctx.chat.module.OnlineUserEntity;

@Service("jedisService")
public class RedisServiceImp implements RedisService {
	
	private static final String SessionKeyOnlineEntity = "Session_Key_Online_Entity";
	private static final String AdminSecurityToken = "0510ee1f655c425e98c6cfda96c153a1";
	private static final String NoPushResponse = "-1";
	
	@Resource
	ChatRedisService chatRedisService;

	@Resource
	JedisConnectionFactory jedisFactory;
	
	@Resource
	ChatDbService chatDbService;
	
	@Resource
	PushService pushService;
	
	@Resource
	ChatUserDao chatUserDao;
	
	ObjectMapper om = new ObjectMapper();

	@Override
	public void online(OnlineBean bean, WebSocketSession session) {
		try{
			if (AdminSecurityToken.equals(bean.getAdminToken())) {
				bean.setAdmin(true);
			} else {
				bean.setAdmin(false);
			}
			OnlineUserEntity entity = new OnlineUserEntity(jedisFactory, bean, session);
			session.getAttributes().put(SessionKeyOnlineEntity, entity);
			chatRedisService.OnlineUser(""+bean.getSelf(), entity);
			chatDbService.OnlineUser(bean);
			if (bean.getReceivePush() != null) {
				chatUserDao.updateReceivePush(bean);
			}
		}catch(Exception e){
			e.printStackTrace();
		}		
	}
	
	@Override
	public void handleTextMessage(WebSocketSession session, String payload) {
		MessageBean message;
		try {
			message = om.readValue(payload, MessageBean.class);
			if (message.getIsPingPong()) {//是pingPang
				return;
			}
			String targetUid = ""+message.getTo();
			OnlineUserEntity entity = onlineUserEntity(session);
			String fromUid = String.valueOf(entity.getBean().getSelf());	
			int unReads = chatRedisService.sendMessageToTarget(entity, targetUid, payload);
			if (unReads > 0) {//用户不在线
				OnlineBean ob = chatUserDao.getUserBean(Integer.parseInt(targetUid));
				if (ob != null) {
					try {
						if (ob.getReceivePush() != null && ob.getReceivePush()) {
							String result = pushService.pushUnread(ob, unReads, fromUid);
							if (NoPushResponse.equals(result)) {//不接受推送
								ob.setReceivePush(false);
								chatUserDao.updateReceivePush(ob);
							}
						}
						
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}
			chatDbService.saveChatMessageToDb(fromUid, message);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void onffline(WebSocketSession session) {
		try{ 
			chatRedisService.OfflineUser(onlineUserEntity(session));
		}catch(Exception e){
			e.printStackTrace();
		}
	}

	@Override
	public void keepalive(WebSocketSession session) {
		chatRedisService.keepalive(onlineUserEntity(session));		
	}
	
	private OnlineUserEntity onlineUserEntity(WebSocketSession session) {
		return (OnlineUserEntity) session.getAttributes().get(SessionKeyOnlineEntity);
	}
	public static void main(String[] args) {
		System.out.println(UUID.randomUUID());
	}
}
