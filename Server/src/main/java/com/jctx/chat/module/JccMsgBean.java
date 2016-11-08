package com.jctx.chat.module;

public class JccMsgBean {
	private final static String PASS_PORT = "200503e7d4dc068825c3bf690262fca3";
	
	int type;
	String message;
	String token;
	String receiverUid;
	
	public static boolean canPass(String t){
		return t != null && t.equals(PASS_PORT);
	}
	
	public boolean validate(){
		return token != null && message != null && message.length() >0;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public String getMessage() {
		return message;
	}

	public void setMessage(String message) {
		this.message = message;
	}

	public String getToken() {
		return token;
	}

	public void setToken(String token) {
		this.token = token;
	}

	public String getReceiverUid() {
		return receiverUid;
	}

	public void setReceiverUid(String receiverUid) {
		this.receiverUid = receiverUid;
	}

	@Override
	public String toString() {
		return "JccMsgBean [type=" + type + ", message=" + message + ", token=" + token + ", receiverUid=" + receiverUid
				+ "]";
	}
}
