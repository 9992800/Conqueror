package com.jctx.chat.dao;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import com.jctx.chat.Utils.ChatException;
import com.jctx.chat.module.ChatGroupMemBean;
import com.jctx.chat.module.ChatUserBean;

@Repository
public class ChatGroupMemDao  extends ChatDao {
	static final String NAME_SPACE = "com.jctx.chat.chatgroup.details";

	public int addMember(int grpId, ChatUserBean bean) throws ChatException {
		Map<String, Object> params = new HashMap<>();
		params.put("groupId", grpId);
		params.put("uid", bean.getUid());
		
		int affected_row =  this.getSqlSession().insert(NAME_SPACE+".addmember", params);
		if (affected_row != 1)
			throw new ChatException("添加用户失败");
		
		return affected_row;
	}

	public int addMembers(int grpId, Integer[] userIdList)  throws SQLException{
		Map<String, Object> params = new HashMap<>();
		params.put("groupId", grpId);
		params.put("uids", userIdList); 
		int affected_row =  this.getSqlSession().insert(NAME_SPACE+".addmembers", params);
		return affected_row;
	}

	public int deleteGroupMembmers(Integer groupId) { 
		return this.getSqlSession().delete(NAME_SPACE+".delAllmembers", groupId);
	}

	public List<ChatGroupMemBean> getAllGroupMembers(int grpId) { 
		return this.getSqlSession().selectList(NAME_SPACE+".listMembers", grpId);
	}

	public int delGroupMembers(int grpId, Integer[] toBeDelUids) {
		Map<String, Object> params = new HashMap<>();
		params.put("groupId", grpId);
		params.put("uids", toBeDelUids);
		return this.getSqlSession().delete(NAME_SPACE+".delmembers", params);
	}
}
