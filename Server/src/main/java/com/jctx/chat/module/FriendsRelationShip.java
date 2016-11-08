package com.jctx.chat.module;

import java.sql.Date;

public class FriendsRelationShip {
	
	public static int FRIENDS_RELATION_STATUS_APPLYING = 0;
	public static int FRIENDS_RELATION_STATUS_NORMAL   = 1;
	public static int FRIENDS_RELATION_STATUS_INBLACK  = 2;
	
	int uid, fuid, status;
	Date createTime, updateTime;
	
	
	public int getUid() {
		return uid;
	}
	public void setUid(int uid) {
		this.uid = uid;
	}
	public int getFuid() {
		return fuid;
	}
	public void setFuid(int fuid) {
		this.fuid = fuid;
	}
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
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
		return "FriendsRelationShip [uid=" + uid + ", fuid=" + fuid + ", status=" + status + ", createTime="
				+ createTime + ", updateTime=" + updateTime + "]";
	} 
}
