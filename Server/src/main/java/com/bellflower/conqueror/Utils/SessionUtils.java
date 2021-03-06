package com.bellflower.conqueror.Utils;

import java.lang.reflect.InvocationTargetException;
import java.text.SimpleDateFormat;
import java.util.HashMap;
import java.util.List;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.http.NameValuePair;
import org.apache.http.client.utils.URLEncodedUtils;
import org.springframework.web.socket.WebSocketSession;

public final class SessionUtils {
	
	public static SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyyMMdd");
	
	public static void parseParam(Object bean, WebSocketSession session) throws IllegalAccessException, InvocationTargetException{
		
		List<NameValuePair> params = URLEncodedUtils.parse(session.getUri(), "UTF-8");
		HashMap<String, Object> map = new HashMap<String, Object>();
		
		for (NameValuePair param : params) {			 
			map.put(param.getName(), param.getValue());
		}		
		map.put("sessionId", session.getId()); 
		
		BeanUtils.populate(bean, map);
	}
}
