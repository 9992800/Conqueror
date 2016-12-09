package com.bellflower.conqueror.service;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;

import javax.annotation.Resource;

import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;
import org.springframework.stereotype.Service;

import com.bellflower.conqueror.module.GameRoomWaitor;
import com.bellflower.conqueror.module.OnlineBean;

@Service
public class GameRoomManager {

	@Resource ThreadPoolTaskExecutor taskExecutor;
	
	//TODO:: 10000 should be replaced
	ConcurrentHashMap<String, GameRoomWaitor> allManagedRooms = 
			new ConcurrentHashMap<String, GameRoomWaitor>(20000);
	
	BlockingQueue<GameRoomWaitor> waitingRoom = 
			new ArrayBlockingQueue<GameRoomWaitor>(20000);

	public void enterRoom(OnlineBean bean) throws InterruptedException {
		if (waitingRoom.isEmpty()){
			GameRoomWaitor gameRoomWaitor = new GameRoomWaitor(); 
			waitingRoom.put(gameRoomWaitor); 
			allManagedRooms.put(gameRoomWaitor.getMapId(), gameRoomWaitor);
			bean.setMapId(gameRoomWaitor.getMapId());
			gameRoomWaitor.enterRoom(bean, this);
			taskExecutor.execute(gameRoomWaitor);
		}else{
			GameRoomWaitor availbel_room = waitingRoom.element();
			bean.setMapId(availbel_room.getMapId());
			availbel_room.enterRoom(bean, this);
		} 
	} 

	public void leaveRoom(OnlineBean bean) throws InterruptedException {
		GameRoomWaitor roomWaitor = allManagedRooms.get(bean.getMapId()); 
		if (null != roomWaitor){
			roomWaitor.leaveRoom(bean, this);
		}	 
	}

	public int getRoomNumbers() { 
		return allManagedRooms.size();
	}

	public void startToPlay(GameRoomWaitor gameRoomWaitor) {
		waitingRoom.remove(gameRoomWaitor);
	}

	public void removeThisRoom(GameRoomWaitor gameRoomWaitor) {
		allManagedRooms.remove(gameRoomWaitor.getMapId());
		waitingRoom.remove(gameRoomWaitor.getMapId());
	}

	public void waitNewPlayer(GameRoomWaitor gameRoomWaitor) throws InterruptedException {
		waitingRoom.put(gameRoomWaitor);
	} 
}
