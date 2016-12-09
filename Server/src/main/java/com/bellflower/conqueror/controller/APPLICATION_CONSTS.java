package com.bellflower.conqueror.controller;

public enum APPLICATION_CONSTS { 
	ONLINE_MESSAGE_REQUEST_TYPE_FIND_COMPONENT(1001),
	ONLINE_MESSAGE_RESPONSE_TYPE_FIND_COMPONENT(3001),
	GAME_STATUS_WAITING(6001),
	GAME_STATUS_PLAYING(6002),
	GAME_STATUS_FINISHED(6003);

	
    private int value;

    APPLICATION_CONSTS(int v) {
        this.value = v;
    }

    public int getValue() {
        return value;
    } 
}
 
