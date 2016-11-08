package com.jctx.chat.service;

import java.sql.SQLException;
import java.util.List;

import javax.annotation.Resource;

import org.springframework.stereotype.Service;

import com.jctx.chat.Utils.ChatException; 
import com.jctx.chat.dao.ChatGroupDao;
import com.jctx.chat.dao.ChatGroupMemDao;
import com.jctx.chat.dao.UserManagerDao;
import com.jctx.chat.module.ChatGroupBean;
import com.jctx.chat.module.ChatGroupMemBean;
import com.jctx.chat.module.ChatUserBean;
import com.mysql.jdbc.exceptions.MySQLIntegrityConstraintViolationException;

@Service
public class ChatGroupService {

	@Resource UserManagerDao userManagerDao;
	@Resource ChatGroupDao chatGroupDao;
	@Resource ChatGroupMemDao chatGroupMemDao;
	
	public boolean checkAuthorize(ChatUserBean bean) {
		
		ChatUserBean beanInDb = userManagerDao.getChatUserInfo(bean.getUid());
		
		if (beanInDb.getGroupNumCreated() >= beanInDb.getGroupRight()){
			return false;
		}
		
		return true;
	}

	public void createChatGroup(ChatUserBean bean, String groupName) throws ChatException { 
		 
		
		int grpId = chatGroupDao.createChatGroup(bean, groupName);
		
		chatGroupMemDao.addMember(grpId, bean);
		
		userManagerDao.addCreateGrpNum(bean.getUid());
	}

	public void addNewMembers(int grpId, Integer[] userIdList) throws ChatException {
		try {
			chatGroupMemDao.addMembers(grpId, userIdList);
			
			chatGroupDao.syncGrpNum(grpId);
			
		} catch (SQLException ex) { 
			ex.printStackTrace();
			if (ex  instanceof MySQLIntegrityConstraintViolationException)
				throw new ChatException("重复的申请");
		}
	}

	public List<ChatGroupBean> getAllGroup(int uid) { 
		return chatGroupDao.getAllGroup(uid);
	}

	public ChatGroupBean getGroupDetails(int uid, Integer groupId) { 
		return chatGroupDao.getGroupDetails(uid, groupId);
	}

	public void deleteGroup(int uid, Integer groupId) throws ChatException {
		
		int affected_row = chatGroupDao.deleteGroup(uid, groupId);
		if (1 != affected_row)
			throw new ChatException("删除群聊失败");
		
		chatGroupMemDao.deleteGroupMembmers(groupId);
		
		userManagerDao.MinusCreateGrpNum(uid);
	}

	public boolean checkGroupLooking(ChatUserBean bean, int grpId) { 
		ChatGroupBean beangrp = chatGroupDao.getGroupDetails(bean.getUid(), grpId);
		return beangrp != null;
	}

	public List<ChatGroupMemBean> getAllGroupMembers(int grpId) { 
		return chatGroupMemDao.getAllGroupMembers(grpId);
	}

	public void delGroupMembers(int grpId, Integer[] toBeDelUids) {
		chatGroupMemDao.delGroupMembers(grpId, toBeDelUids);
	}

	public List<ChatGroupBean> listGroups(int startGrpId) {
		return chatGroupDao.listGroups(startGrpId);
	}

}