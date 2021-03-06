package com.bellflower.conqueror.Utils;

import java.util.HashMap;

public final class ChatErrorCode {
	public static final int CODE_SUCCESS = 10000;
	public static final String DESC_SUCCESS = "成功";
	
	public static final int CODE_COMMON_ERROR = 10001;

	public static final int CODE_BAD_REQUEST = 10002;
	public static final String DESC_BAD_REQUEST = "请求参数错误:";
	
	public static final int CODE_NO_RIGHT = 20001;
	public static final String DESC_NO_RIGHT = "无权访问该接口";
	
	public static final int CODE_SIGN_FAILED = 20002;
	public static final String DESC_SIGN_FAILED = "签名校验失败";
	
	public static final int CODE_HTTP_SESSION_TIMEOUT = 20003;
	public static final String DESC_HTTP_SESSION_TIMEOUT = "您的会话超时,请重新请求";
	

	public static final int CODE_SYSTEM_ERROR = -1;
	public static final String DESC_SYSTEM_ERROR = "系统错误，请稍后重试";
	
	static final HashMap<Integer, String> errorMap = new HashMap<Integer, String>();
	
	static{
		errorMap.put(CODE_SUCCESS, DESC_SUCCESS);
		errorMap.put(CODE_SYSTEM_ERROR, DESC_SYSTEM_ERROR);
	}
}
