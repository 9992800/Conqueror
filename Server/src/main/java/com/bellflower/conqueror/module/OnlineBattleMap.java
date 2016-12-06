package com.bellflower.conqueror.module;

import java.util.List;

import org.json.JSONArray;

public class OnlineBattleMap {
	String id;
	int playerNum;
	List<OnlineBean> players;
	JSONArray	mapData;
	public int getPlayerNum() {
		return playerNum;
	}

	public void setPlayerNum(int playerNum) {
		this.playerNum = playerNum;
	}

	public List<OnlineBean> getPlayers() {
		return players;
	}

	public void setPlayers(List<OnlineBean> players) {
		this.players = players;
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
