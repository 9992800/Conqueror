/**
 * 
 */
package com.bellflower.conqueror.Utils;

import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.WebSocketSession;

/**
 * 聊天数据存储共用类
 * @author Leo
 * 2016-10-26
 */
@Component
@SuppressWarnings("unchecked")
public class ChatRuntimeUtil{
	private static final Logger log = LoggerFactory.getLogger(ChatRuntimeUtil.class);
	private static final String sessionKeyLastActiveTime = "last_active_time_stamp";
	/**
	 * 超过该时间（单位：ms）未发送或者接受消息的session将被关闭
	 */
	private static final long timeMillisToKeepAlive = 3600000;//一小时

	/**
	 * 保存webSocketSession的数据结构，key为uid，value为session或者List<session>
	 */
	private static final Map<Long, Object> sessionMap = new ConcurrentHashMap<Long, Object>(10000);
	
	/**
	 * 添加session
	 * @param uid
	 * @param session
	 */
	
	public static void addSession(Long uid, WebSocketSession session) {
		Object obj = sessionMap.get(uid);
		if (obj != null) {//非空，可能为多点登陆
			if (obj instanceof WebSocketSession) {//为session
				List<WebSocketSession> ss = Collections.synchronizedList(new LinkedList<WebSocketSession>());
				ss.add((WebSocketSession) obj);
				ss.add(session);
				sessionMap.put(uid, ss);
			} else {//为list
				List<WebSocketSession> ss = (List<WebSocketSession>) obj;
				ss.add(session);
			}
		} else {//初次登陆
			sessionMap.put(uid, session);
		}		
		updateSessionActive(session);
	}
	/**
	 * 删除session但不调用session.close
	 * @param uid
	 */
	public static void removeSession(Long uid, WebSocketSession session) {
		Object obj = sessionMap.get(uid);
		if(obj != null) {
			if (obj instanceof WebSocketSession) {//为session
				if (obj.equals(session)) {
					sessionMap.remove(uid);
				}
			} else {//为list
				List<WebSocketSession> ss = (List<WebSocketSession>) obj;
				ss.remove(session);
				if (ss.isEmpty()) {
					sessionMap.remove(uid);
				}
			}
		} else {
			sessionMap.remove(uid);
		}
	}
	/**
	 * 更新session活动时间
	 */
	public static void updateSessionActive(WebSocketSession s) {
		if (s != null && s.isOpen()) {
			s.getAttributes().put(sessionKeyLastActiveTime, System.currentTimeMillis());
		} else {
			log.error("session has already been closed, sid:{}", s.getId());
		}
	}
	/**
	 * 定时器：检测session多长时间未活动
	 */
	@Scheduled(fixedRate=15*60*1000)   //每15分钟执行一次
	public void testSessionActive() {
		Iterator<Object> it = sessionMap.values().iterator();
		while (it.hasNext()) {
			Object obj = it.next();
			if (obj != null) {
				try {
					if (obj instanceof WebSocketSession) {
						tryCloseSession((WebSocketSession) obj);
					} else {
						List<WebSocketSession> list = (List<WebSocketSession>) obj;
						Iterator<WebSocketSession> it2 = list.iterator();
						while (it2.hasNext()) {
							tryCloseSession(it2.next());
						}
					}
				} catch (Exception e) {
					log.error("ChatRuntimeUtil testSessionAlive error!", e);
				}
			}
		}
	}
	
	private void tryCloseSession(WebSocketSession session) {
		try {
			long current = System.currentTimeMillis();
			Long last = (Long) session.getAttributes().get(sessionKeyLastActiveTime);
			if (current - last >= timeMillisToKeepAlive) {
				log.debug("close deactive session:{}", session.getId());
				session.close();
			}
		} catch (Exception e) {
			log.error("close session error, session id:{}", session.getId());
		}
		
	}
}
