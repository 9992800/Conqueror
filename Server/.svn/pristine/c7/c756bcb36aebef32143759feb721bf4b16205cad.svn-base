package com.jctx.chat.dao;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import com.jctx.chat.Utils.ChatException;
import com.jctx.chat.module.ChatGroupBean;
import com.jctx.chat.module.ChatUserBean;

@Repository
public class ChatGroupDao extends ChatDao {
	
	static final String NAME_SPACE = "com.jctx.chat.chatgroup.base";

	public int createChatGroup(ChatUserBean bean, String groupName) throws ChatException {
		Map<String, Object> params = new HashMap<>();
		params.put("uid", bean.getUid());
		params.put("grpName", groupName);
		int affected_row =  this.getSqlSession().insert(NAME_SPACE+".create", params);
		if (affected_row != 1)
			throw new ChatException("创建聊天群组失败");
		
		return (int) params.get("groupId");
	}  

	public List<ChatGroupBean> getAllGroup(int uid) { 
		List<ChatGroupBean> list = this.getSqlSession().selectList(NAME_SPACE + ".groupList", uid);
		if (null == list){
			list = new ArrayList<>();
		}
		return list;
	}

	public ChatGroupBean getGroupDetails(int uid, Integer groupId) { 
		Map<String, Object> params = new HashMap<>();
		params.put("groupId", groupId);
		params.put("uid", uid); 
		return this.getSqlSession().selectOne(NAME_SPACE + ".groupDetails", params); 
	}

	public int deleteGroup(int uid, Integer groupId) {
		Map<String, Object> params = new HashMap<>();
		params.put("groupId", groupId);
		params.put("uid", uid); 
		return this.getSqlSession().update(NAME_SPACE + ".deleteGroup", params);
	}

	public int syncGrpNum(int grpId) {
		return this.getSqlSession().update(NAME_SPACE + ".syncGrpNum", grpId);
	}

	public List<ChatGroupBean> listGroups(int startGrpId) { 
		return this.getSqlSession().selectList(NAME_SPACE + ".listGroups", startGrpId);
	}
}
