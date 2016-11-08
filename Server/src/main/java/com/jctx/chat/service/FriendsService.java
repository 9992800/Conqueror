package com.jctx.chat.service;

import java.sql.SQLException;
import java.util.List;

import javax.annotation.Resource;

import org.springframework.stereotype.Service;

import com.jctx.chat.Utils.ChatException;
import com.jctx.chat.dao.ChatMsgDao;
import com.jctx.chat.dao.FriendsApplyDao;
import com.jctx.chat.dao.FriendsRelationDao;
import com.jctx.chat.module.ChatUserBean;
import com.jctx.chat.module.FriendsApplication;
import com.jctx.chat.module.FriendsRelationShip;
import com.jctx.chat.module.MessageBean;
import com.mysql.jdbc.exceptions.MySQLIntegrityConstraintViolationException;

@Service
public class FriendsService {

	@Resource FriendsApplyDao friendsApplyDao;
	
	@Resource FriendsRelationDao friendsRelationDao; 
	
	@Resource ChatMsgDao chatMsgDao;
	
	public void applyAsFriends(ChatUserBean applicant, int targetUid) throws ChatException {
		
		try { 
			
			friendsApplyDao.applyFriends(applicant, targetUid);
			
			friendsRelationDao.recordThisApply(applicant, targetUid);
			
		}catch (SQLException ex) {
			if (ex  instanceof MySQLIntegrityConstraintViolationException)
				throw new ChatException("重复的申请");
		}
		
	} 
	
	public void approvedAsFriends(ChatUserBean bean, int applicantUid) throws ChatException{
		
		int affected_row = friendsApplyDao.gotApproved(bean, applicantUid);
		if (1 != affected_row)
			throw new ChatException("更新申请状态失败");
		
		affected_row = friendsRelationDao.approveIt(bean, applicantUid);
		if (1 != affected_row)
			throw new ChatException("添加好友失败");
		
		affected_row = friendsRelationDao.addToMyRelation(bean, applicantUid);
		
		if (1 != affected_row)
			throw new ChatException("添加好友失败");
	} 

	public List<FriendsApplication> getAllApplicants(ChatUserBean bean) {  
		return friendsApplyDao.getAllApplicants(bean);
	}

	public List<FriendsRelationShip> getAllMyFriends(ChatUserBean bean) { 
		return friendsRelationDao.getAllMyFriends(bean);
	}

	public void removeMyFriends(ChatUserBean bean, int toBeDelUid) throws ChatException {
		int affected_row =friendsRelationDao.removeOneFriend(bean, toBeDelUid);
		if (1 != affected_row)
			throw new ChatException("删除失败");
	}

	public void blockSomeOne(ChatUserBean bean, int toBlackUid) throws ChatException {
		int affected_row =friendsRelationDao.blockSomeOne(bean, toBlackUid);
		if (1 > affected_row)
			throw new ChatException("添加黑名单失败");
	}

	public List<FriendsRelationShip> getAllInBlack(ChatUserBean bean) { 
		return friendsRelationDao.getAllInBlack(bean);
	}

	public void removeFromBlack(ChatUserBean bean, int toBeDelUid) throws ChatException {
		int affected_row =friendsRelationDao.removeFromBlack(bean, toBeDelUid);
		if (1 != affected_row)
			throw new ChatException("删除失败");
	}

	public List<MessageBean> getHistoryMsgOfFriends(ChatUserBean bean, int peerUid, int startMsgId) {
		return chatMsgDao.getHistoryMessage(bean, peerUid, startMsgId);
	}
}
