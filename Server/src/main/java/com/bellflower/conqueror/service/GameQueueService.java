package com.bellflower.conqueror.service;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import javax.annotation.Resource;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.stereotype.Service;

import com.bellflower.conqueror.module.OnlineBattleMap;
import com.bellflower.conqueror.module.OnlineBean;

@Service
public class GameQueueService {
	
	@Resource BattleFiledService battleFiledService;
	
	public static final int max_waitor_in_queue = 100;
	public static final int finding_status_wait = 1;
	public static final int finding_status_create = 2;
	
	BlockingQueue<OnlineBean> waitingQueue = new ArrayBlockingQueue<OnlineBean>(max_waitor_in_queue);
	public JSONObject findOppoent(OnlineBean me) throws InterruptedException {

		if (waitingQueue.size() >= 3){
			OnlineBean waitor1 = (OnlineBean) waitingQueue.take();
			OnlineBean waitor2 = (OnlineBean) waitingQueue.take();
			List<OnlineBean> data = new ArrayList<>();
			data.add(waitor1);
			data.add(waitor2);
			data.add(me);

			JSONArray payer_ids = new JSONArray();
			payer_ids.put(waitor1.getUserId());
			payer_ids.put(waitor2.getUserId());
			payer_ids.put(me.getUserId());
			
			OnlineBattleMap map = battleFiledService.createOnlineBattle(data);
			JSONObject result = new JSONObject();
			result.put("payer_ids", payer_ids);
			result.put("player_num", data.size());
			result.put("status", finding_status_create);
			result.put("map_id", map.getId());				
			return result;
			
		}else{
			
			JSONObject result = new JSONObject();
			result.put("result", waitingQueue.offer(me));
			result.put("status", finding_status_wait);
			result.put("wait_num", waitingQueue.size()); 			
			return result;
		}
	} 
}
