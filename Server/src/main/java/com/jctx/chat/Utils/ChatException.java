package com.jctx.chat.Utils;

import java.util.Map;
 

public class ChatException extends Exception { 
	
	private static final long serialVersionUID = -2493280170070533519L;
	Map<String, Object> what;
	
	public ChatException(){
		super();
	}
	
	public ChatException(String message){
		super(message);
		what = ResultMapUtils.errorWithCode(ChatErrorCode.CODE_COMMON_ERROR, message); 
	}
	
	public ChatException(Integer code, String message){
		super(message);
		what = ResultMapUtils.errorWithCode(code, message); 
	}

	public Map<String, Object> getWhat() { 
		return this.what;
	}
}
