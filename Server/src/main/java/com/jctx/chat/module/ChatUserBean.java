package com.jctx.chat.module;

import java.sql.Date;

public class ChatUserBean {
	int uid, level, groupRight, groupNumCreated;
	String userName;
	Date createTime, updateTime;
	
	public int getLevel() {
		return level;
	}
	public void setLevel(int level) {
		this.level = level;
	}
	public int getGroupRight() {
		return groupRight;
	}
	public void setGroupRight(int groupRight) {
		this.groupRight = groupRight;
	}
	public String getUserName() {
		return userName;
	}
	public void setUserName(String userName) {
		this.userName = userName;
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
	public int getUid() {
		return uid;
	}
	public void setUid(int uid) {
		this.uid = uid;
	} 
	
	public int getGroupNumCreated() {
		return groupNumCreated;
	}
	public void setGroupNumCreated(int groupNumCreated) {
		this.groupNumCreated = groupNumCreated;
	}
	@Override
	public String toString() {
		return "ChatUserBean [uid=" + uid + ", level=" + level + ", groupRight=" + groupRight + ", groupNumCreated="
				+ groupNumCreated + ", userName=" + userName + ", createTime=" + createTime + ", updateTime="
				+ updateTime + "]";
	} 
}
