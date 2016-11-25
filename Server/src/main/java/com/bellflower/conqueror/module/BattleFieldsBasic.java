package com.bellflower.conqueror.module;

import java.util.ArrayList;
import java.util.List;

public class BattleFieldsBasic {
	
	public static final int DEFAULT_PLAYER_NUM = 3;
	
	String 		id;	
	int 		playerNum;
	int 		status; 
	long		createTime;
	private String owner;
	String ownerName;
	List<String> players = new ArrayList<String>(3);
	
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


	public void setOwnerName(String user_name) {
		this.ownerName = user_name;
	}
	public String getOwnerName(){
		return this.ownerName;
	}
	
	
	public List<String> getPlayer(){
		return this.players;
	}
	public void setPlayer(List<String> p) {
		this.players = p;
	}


	public void addPlayer(String user_id) {
		this.players.add(user_id);
	}
}
