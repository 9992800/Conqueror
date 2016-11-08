/**
 * 
 */
package com.jctx.chat.service;

import java.net.URLEncoder;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.jctx.chat.Utils.HttpUtils;
import com.jctx.chat.module.OnlineBean;

/**
 * <pre>
 * 推送service实现
 * @author Leo
 * 2016-11-3
 * </pre>
 */
@Service("pushService")
public class PushServiceImpl implements PushService {

	private static final Logger log = LoggerFactory.getLogger(PushServiceImpl.class);
	private static final String query = "os=%s&uid=%s&content=%s&page=%s&id=%s&token=%s";
	private static final String unreadTip = "您有%d条未读短消息";
	
	@Value("${push.url}")
	private String url;
	
	@Value("${push.token}")
	private String token;
	
	@Override
	public String pushUnread(OnlineBean ob, int msgNum, String from) throws Exception {
		if (ob.isAdmin()) {
			return null;
		}
		String os = "";
		if (ob.getDevice() == 1) {//ios
			os = "ios";
		} else if (ob.getDevice() == 2) {//android
			os = "android";
		}
		return push(os, String.valueOf(ob.getSelf()), String.format(unreadTip, msgNum), "chat_list", from);
	}

	private String push(String os, String uid, String content, String page, String id) {
		String q = null;
		try {
			q = String.format(query, os, uid, URLEncoder.encode(content, "utf-8"), page, id, token);
			//q = URLEncoder.encode(q, "utf-8");
			String response = HttpUtils.get(url, q);
			log.warn(response);
			return response;
		} catch (Exception e) {
			log.error("PushServiceImpl push error", e);
			log.error("query:{},url:{}",q,url);
		}
		return null;
	}
}
