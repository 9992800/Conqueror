package com.bellflower.conqueror.service;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;
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
	
	ConcurrentHashMap<String, GameRoomWaitor> roomLeader = 
			new ConcurrentHashMap<String, GameRoomWaitor>(10000);
	
	static final BlockingQueue<OnlineBean> threeWaitingTable = 
			new ArrayBlockingQueue<>(3);
	
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

	public void enterRoom(OnlineBean bean) throws InterruptedException { 
		threeWaitingTable.put(bean);
		if (threeWaitingTable.size() >= 3){
			this.createGameRoom();
		}
	}

	private void createGameRoom() throws InterruptedException {		 
		
		
		List<OnlineBean> data = new ArrayList<>(threeWaitingTable);
		threeWaitingTable.clear();
		
		GameRoomWaitor gameRoomWaitor = new GameRoomWaitor(data);  		
		taskExecutor.execute(gameRoomWaitor);
		
		roomLeader.put(gameRoomWaitor.getMapId(), gameRoomWaitor);
	}

	public void leaveRoom(OnlineBean bean) { 
		threeWaitingTable.remove(bean);
	}
}
