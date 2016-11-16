package com.bellflower.conqueror.module;

public class GameServer {
	String		id;
	int 		playerNum;
	String  	serverName;
	
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	
	
	public int getPlayerNum() {
		return playerNum;
	}
	public void setPlayerNum(int playerNum) {
		this.playerNum = playerNum;
	}
	
	
	public String getServerName() {
		return serverName;
	}
	public void setServerName(String serverName) {
		this.serverName = serverName;
	}
	
	@Override
	public String toString() {
		return "GameServer [id=" + id + ", playerNum=" + playerNum + ", serverName=" + serverName + "]";
	}	
}
