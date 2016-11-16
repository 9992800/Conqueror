package com.bellflower.conqueror.module;

public class BattleFields {
	String 		id;
	String 		serverId;
	int 		playerNum;
	int 		status;
	String		mapId;
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getServerId() {
		return serverId;
	}
	public void setServerId(String serverId) {
		this.serverId = serverId;
	}
	public int getPlayerNum() {
		return playerNum;
	}
	public void setPlayerNum(int playerNum) {
		this.playerNum = playerNum;
	}
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	public String getMapId() {
		return mapId;
	}
	public void setMapId(String mapId) {
		this.mapId = mapId;
	}
	
	@Override
	public String toString() {
		return "BattleFields [id=" + id + ", serverId=" + serverId + ", playerNum=" + playerNum + ", status=" + status
				+ ", mapId=" + mapId + "]";
	}
}
