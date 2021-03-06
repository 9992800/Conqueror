package com.bellflower.conqueror.service;

import javax.annotation.Resource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

@Service
public class TimerService {
	private static final Logger logger =
		    LoggerFactory.getLogger(TimerService.class);
	
	@Resource GameDispatchService gameDispatchService;
	@Resource GameRoomManager roomManager;
	
	@Scheduled(fixedRate=1000)
	public void prepairGameRoom(){  
		logger.info("waiting in room:"+roomManager.getRoomNumbers());
		logger.info("online users:"+gameDispatchService.getOnlineUsers());
	}
}
