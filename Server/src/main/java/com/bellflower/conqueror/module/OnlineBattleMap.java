package com.bellflower.conqueror.module;

import org.json.JSONArray;

public class OnlineBattleMap {
	String id;
	int playerNum; 
	JSONArray	mapData;
	public int getPlayerNum() {
		return playerNum;
	}

	public void setPlayerNum(int playerNum) {
		this.playerNum = playerNum;
	} 
	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public void setMapData(JSONArray cellData) {
		this.mapData = cellData;		
	}

	public JSONArray getMapData() {
		return mapData;
	}	
}
