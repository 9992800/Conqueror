package com.jctx.chat.dao;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import com.jctx.chat.module.ChatUserBean;

@Repository
public class UserManagerDao extends ChatDao {

	static final String NAME_SPACE = "com.jctx.chat.usermanager";
	
	public ChatUserBean getChatUserInfo(int uid) { 
		return this.getSqlSession().selectOne(NAME_SPACE+".getUser", uid);
	}

	public int createChatUser(ChatUserBean bean) {
		return this.getSqlSession().insert(NAME_SPACE+".createUser", bean);
	}

	public int updateChatUser(Map<String, Object> params) {
		return this.getSqlSession().update(NAME_SPACE+".updateUser", params);
	}

	public int MinusCreateGrpNum(int uid) {
		Map<String, Object> params = new HashMap<>();
		params.put("grpNum", -1);
		params.put("uid", uid);
		return this.updateChatUser(params);
	}

	public int addCreateGrpNum(int uid) { 
		Map<String, Object> params = new HashMap<>();
		params.put("grpNum", -1);
		params.put("uid", uid);
		return this.updateChatUser(params);
	}

	public List<ChatUserBean> getChatUsers(int startUid) {
		return this.getSqlSession().selectList(NAME_SPACE + ".listUsers", startUid);
	}

}
