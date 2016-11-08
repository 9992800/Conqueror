/**
 * 
 */
package com.jctx.chat.service;

import com.jctx.chat.module.OnlineBean;

/**
 * <pre>
 * 推送service
 * @author Leo
 * 2016-11-3
 * </pre>
 */
public interface PushService {

	/**
	 * 推送一条消息：“您有未读的短消息”
	 * @param ob
	 * @return
	 * @throws Exception
	 */
	public String pushUnread(OnlineBean ob, int msgNum, String from) throws Exception;
}
