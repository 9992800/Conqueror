package com.jctx.chat.dao;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.springframework.stereotype.Repository;

import com.jctx.chat.module.ChatUserBean;
import com.jctx.chat.module.FriendsApplication;

@Repository
public class FriendsApplyDao extends ChatDao {

	private static final String NAME_SPACE = "com.jctx.chat.friends.apply";
	
	public int applyFriends(ChatUserBean applicant, int targetUid) throws SQLException{
		
		HashMap<String , Object> params = new HashMap<>();
		params.put("fromUid", applicant.getUid());
		params.put("toUid", targetUid);
		
		return this.getSqlSession().insert(NAME_SPACE+".addApply", params);
	}

	public int gotApproved(ChatUserBean bean, int applicantUid) {
		HashMap<String , Object> params = new HashMap<>();
		params.put("fromUid", applicantUid);
		params.put("toUid", bean.getUid());
		
		return this.getSqlSession().insert(NAME_SPACE+".gotApproved", params);
	} 

	public List<FriendsApplication> getAllApplicants(ChatUserBean bean) {
		
		List<FriendsApplication> list = this.getSqlSession().selectList(NAME_SPACE+".getApplicantToMe", bean.getUid());
		
		if (null == list){
			list = new ArrayList<>(); 
		}
		
		return list;
	} 
}
