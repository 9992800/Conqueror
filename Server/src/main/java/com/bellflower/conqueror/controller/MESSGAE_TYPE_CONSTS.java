package com.bellflower.conqueror.controller;

public enum MESSGAE_TYPE_CONSTS { 
	ONLINE_MESSAGE_REQUEST_TYPE_FIND_COMPONENT(1001),
	ONLINE_MESSAGE_RESPONSE_TYPE_FIND_COMPONENT(3001);

    private int type;

    MESSGAE_TYPE_CONSTS(int type) {
        this.type = type;
    }

    public int getType() {
        return type;
    } 
}
 
