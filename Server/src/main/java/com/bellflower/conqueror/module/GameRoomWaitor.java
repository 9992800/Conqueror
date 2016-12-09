package com.bellflower.conqueror.module;

import java.util.ArrayDeque;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import javax.annotation.Resource;

import com.bellflower.conqueror.controller.APPLICATION_CONSTS;
import com.bellflower.conqueror.service.BattleFiledService;
import com.bellflower.conqueror.service.GameRoomManager;

public class GameRoomWaitor implements Runnable {

	@Resource BattleFiledService battleFiledService;
	@Resource GameRoomManager gameRoomManager;
	
	private int __gameStatus = APPLICATION_CONSTS.GAME_STATUS_WAITING.getValue();
	private OnlineBattleMap __theBattleMap;
	private Queue<String> __broadCaster = new ArrayDeque<>();
	BlockingQueue<OnlineBean> threeWaitingTable = 
			new ArrayBlockingQueue<>(3);
	
	public GameRoomWaitor() {
		__theBattleMap = battleFiledService.createOnlineBattle(3);
	}

	@Override
	public void run() {
		while (!threeWaitingTable.isEmpty()){
			try {
				while (!__broadCaster.isEmpty()) {
					String msg = (String) __broadCaster.poll();			
				}
				this.wait();
				 
			} catch (InterruptedException e) { 
				e.printStackTrace();
			}
		}
	} 
	
	public String getMapId() { 
		return __theBattleMap.getId();
	}

	public void tikiTakaMsg(String msg){
		__broadCaster.add(msg);
		this.notify();
	} 

	public void enterRoom(OnlineBean bean, GameRoomManager gameRoomManager) throws InterruptedException {
		threeWaitingTable.put(bean);
		if (0 == threeWaitingTable.remainingCapacity()){
			gameRoomManager.startToPlay(this);			
		}else{
			//TODO::broad cast new user enter.
		}
	}

	public void leaveRoom(OnlineBean bean, GameRoomManager gameRoomManager) throws InterruptedException {
		threeWaitingTable.remove(bean);
		if (threeWaitingTable.isEmpty()){
			gameRoomManager.removeThisRoom(this);
		}else{
			
			if (__gameStatus == APPLICATION_CONSTS.GAME_STATUS_PLAYING.getValue()){
				//TODO:: manage the left guy
			}else{
				gameRoomManager.waitNewPlayer(this);
			}
		}
	}
}
