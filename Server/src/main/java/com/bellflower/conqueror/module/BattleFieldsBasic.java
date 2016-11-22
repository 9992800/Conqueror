package com.bellflower.conqueror.module;

import org.json.JSONArray;

public class BattleFieldsBasic {
	
	public static final int DEFAULT_PLAYER_NUM = 3;
	
	String 		id;	
	int 		playerNum;
	int 		status; 
	long		createTime;
	private String owner;
	
	public BattleFieldsBasic(){
		this.createTime = System.currentTimeMillis();
		this.playerNum = DEFAULT_PLAYER_NUM;
	}
	
	
	public String getId() {
		return id;
	}
	public void setId(String bi) {
		this.id = bi;
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
	public String getOwner(){
		return this.owner;
	}
	public void setOwner(String user_id) {
		 this.owner = user_id;
	}
}
