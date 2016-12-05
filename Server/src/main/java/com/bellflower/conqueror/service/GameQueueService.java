package com.bellflower.conqueror.service;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import javax.annotation.Resource;

import org.json.JSONObject;
import org.springframework.stereotype.Service;

import com.bellflower.conqueror.module.OnlineBean;

@Service
public class GameQueueService {
	
	@Resource BattleFiledService battleFiledService;
	
	public static final int max_waitor_in_queue = 100;
	public static final int finding_status_wait = 1;
	
	BlockingQueue<OnlineBean> waitingQueue = new ArrayBlockingQueue<OnlineBean>(max_waitor_in_queue);
	public JSONObject findOppoent(OnlineBean me) throws InterruptedException {

		if (waitingQueue.size() >= 10){
			OnlineBean waitor1 = (OnlineBean) waitingQueue.take();
			OnlineBean waitor2 = (OnlineBean) waitingQueue.take();
			List<OnlineBean> data = new ArrayList<>();
			data.add(waitor1);
			data.add(waitor2);
			data.add(me);
			return battleFiledService.createBattle(data);
			
		}else{
			
			JSONObject result = new JSONObject();
			result.put("result:", waitingQueue.offer(me));
			result.put("status", finding_status_wait);
			result.put("wait_num", waitingQueue.size()); 			
			return result;
		}
	}
	
	
	public void enqueue() {
	}

}
