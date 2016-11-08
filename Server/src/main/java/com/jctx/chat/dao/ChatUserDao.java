package com.jctx.chat.dao;

import org.springframework.stereotype.Repository;

import com.jctx.chat.module.OnlineBean;

@Repository
public class ChatUserDao  extends ChatDao {
	
	static final String NAME_SPACE = "com.jctx.chat.user";

	public void newOnlineUser(OnlineBean bean) {
		this.getSqlSession().insert(NAME_SPACE+".newUser", bean);
	}

	public void updateOnlineUser(OnlineBean bean) {
		this.getSqlSession().update(NAME_SPACE+".updateUser", bean);
	} 

	public OnlineBean getUserBean(int self) {
		OnlineBean beanInDb = this.getSqlSession().selectOne(NAME_SPACE+".getUser", self);
		return beanInDb;
	}
	
	public void updateReceivePush(OnlineBean bean) {
		this.getSqlSession().update(NAME_SPACE+".updateReceivePush", bean);
	}
}
