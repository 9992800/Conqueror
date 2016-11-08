package com.jctx.chat.dao;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.springframework.stereotype.Repository;

import com.jctx.chat.module.ChatUserBean;
import com.jctx.chat.module.FriendsRelationShip;

@Repository
public class FriendsRelationDao extends ChatDao {

	private static final String NAME_SPACE = "com.jctx.chat.friends.relation";
	
	public int recordThisApply(ChatUserBean applicant, int targetUid) {
		
		HashMap<String , Object> params = new HashMap<>();
		params.put("uid", applicant.getUid());
		params.put("fuid", targetUid);
		
		return this.getSqlSession().insert(NAME_SPACE+".recordApply", params);
	}

	public int approveIt(ChatUserBean bean, int applicantUid) {
		HashMap<String , Object> params = new HashMap<>();
		params.put("uid", applicantUid);
		params.put("fuid", bean.getUid());
		
		return this.getSqlSession().update(NAME_SPACE+".approveIt", params);
	}

	public int addToMyRelation(ChatUserBean bean, int applicantUid) {
		HashMap<String , Object> params = new HashMap<>();
		params.put("uid", bean.getUid());
		params.put("fuid", applicantUid);
		
		return this.getSqlSession().insert(NAME_SPACE+".addAsNewFriend", params);
	}

	public List<FriendsRelationShip> getAllMyFriends(ChatUserBean bean) { 
		List<FriendsRelationShip> list = this.getSqlSession().selectList(NAME_SPACE+".getAllMyFriends", bean.getUid());
		
		if (null == list){
			list = new ArrayList<>();
		}
		return list;
	}

	public int removeOneFriend(ChatUserBean bean, int toBeDelUid) {
		
		HashMap<String , Object> params = new HashMap<>();
		params.put("uid", bean.getUid());
		params.put("fuid", toBeDelUid);
		
		return this.getSqlSession().delete(NAME_SPACE+".delMyFriend", params);
	}

	public int blockSomeOne(ChatUserBean bean, int toBlackUid) {
		HashMap<String , Object> params = new HashMap<>();
		params.put("uid", bean.getUid());
		params.put("fuid", toBlackUid);
		
		return this.getSqlSession().update(NAME_SPACE+".blockSomeOne", params);
	}

	public List<FriendsRelationShip> getAllInBlack(ChatUserBean bean) {
		List<FriendsRelationShip> list = this.getSqlSession().selectList(NAME_SPACE+".getAllInBlack", bean.getUid());
		
		if (null == list){
			list = new ArrayList<>();
		}
		
		return list;
	}

	public int removeFromBlack(ChatUserBean bean, int toBeDelUid) {
		HashMap<String , Object> params = new HashMap<>();
		params.put("uid", bean.getUid());
		params.put("fuid", toBeDelUid);
		
		return this.getSqlSession().update(NAME_SPACE+".removeFromBlack", params);
	}
}
