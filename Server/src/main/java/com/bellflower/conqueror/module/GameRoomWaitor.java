package com.bellflower.conqueror.module;

import java.util.ArrayDeque;
import java.util.List;
import java.util.Queue;

import javax.annotation.Resource;

import com.bellflower.conqueror.service.BattleFiledService;

public class GameRoomWaitor implements Runnable {

	@Resource BattleFiledService battleFiledService;
	
	private OnlineBattleMap __theBattleMap;
	private Queue<String> __broadCaster = new ArrayDeque<>();
	
	public GameRoomWaitor(List<OnlineBean>data){
		__theBattleMap = battleFiledService.createOnlineBattle(data);
	}
	
	@Override
	public void run() {
		while (!__broadCaster.isEmpty()) {
			String msg = (String) __broadCaster.poll();			
		}
		try {
			this.wait();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public String getMapId() { 
		return __theBattleMap.getId();
	}

	public void tikiTakaMsg(String msg){
		__broadCaster.add(msg);
		this.notify();
	}
}
