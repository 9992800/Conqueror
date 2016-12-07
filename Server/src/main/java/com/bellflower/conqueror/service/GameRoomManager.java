package com.bellflower.conqueror.service;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;
import org.springframework.stereotype.Service;
import com.bellflower.conqueror.module.GameRoomWaitor;
import com.bellflower.conqueror.module.OnlineBean;

@Service
public class GameRoomManager {

	@Resource ThreadPoolTaskExecutor taskExecutor;
	@Resource Map<String, GameRoomWaitor> roomLeader = new HashMap<>(10000);
	
	public JSONObject createGameRoom(List<OnlineBean> data) {
		JSONObject result = new JSONObject();
		JSONArray payer_ids = new JSONArray();
		for (int i = 0; i < data.size(); i++){
			OnlineBean bean = data.get(i);
			payer_ids.put(bean.getUserId());
		} 
		
		GameRoomWaitor gameRoomWaitor = new GameRoomWaitor(data); 
		result.put("payer_ids", payer_ids);
		result.put("player_num", data.size());
		result.put("room_id", gameRoomWaitor.getMapId());		
		
		taskExecutor.execute(gameRoomWaitor);
		
		roomLeader.put(gameRoomWaitor.getMapId(), gameRoomWaitor);
		
		return result;
	}
}
