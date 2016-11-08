package com.jctx.chat.module;

import java.sql.Date;

public class ChatGroupMemBean {
	int  groupId, uid;
	Date createTime, updateTime;
	 
	public int getGroupId() {
		return groupId;
	}
	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}
	public int getUid() {
		return uid;
	}
	public void setUid(int uid) {
		this.uid = uid;
	}
	public Date getCreateTime() {
		return createTime;
	}
	public void setCreateTime(Date createTime) {
		this.createTime = createTime;
	}
	public Date getUpdateTime() {
		return updateTime;
	}
	public void setUpdateTime(Date updateTime) {
		this.updateTime = updateTime;
	}
	@Override
	public String toString() {
		return "ChatGroupMemBean [groupId=" + groupId + ", uid=" + uid + ", createTime=" + createTime + ", updateTime="
				+ updateTime + "]";
	} 
	
	
}
