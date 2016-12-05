package com.bellflower.conqueror.service;

import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

@Service
public class TimerService {
	@Scheduled(fixedRate=2000)
	public void findOldWaitor(){
		System.out.println("Count Student...");
	}
}
