package com.bellflower.conqueror.module;

import org.json.JSONArray;

public class BattleFieldsMap {

	String 		id;
	private JSONArray cellData;
	private String userId;

	public JSONArray getDataInfo(){
		return this.cellData;
	}
	public void setDataInfo(JSONArray cells) {
		 this.cellData = cells;
	}
	public String getId() {
		return id;
	}
	public void setId(String bi) {
		this.id = bi;
	}
	public void setUserId(String user_id) {
		this.userId = user_id;
	}
	public String getUserId(){
		return this.userId;
	}
}
