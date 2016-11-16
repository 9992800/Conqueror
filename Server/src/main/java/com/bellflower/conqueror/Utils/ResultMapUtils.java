package com.bellflower.conqueror.Utils;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONObject;
 

public final class ResultMapUtils {
	
	public static Map<String, Object> success() {
		Map<String, Object> success = new HashMap<String, Object>();
		success.put("apicode", ChatErrorCode.CODE_SUCCESS);
		success.put("message", 
				ChatErrorCode.DESC_SUCCESS); 
		return success;
	}
	
	public static Map<String, Object> success(Object data) {
		Map<String, Object> success = new HashMap<String, Object>();
		success.put("apicode", ChatErrorCode.CODE_SUCCESS);
		success.put("message", 
				ChatErrorCode.DESC_SUCCESS); 

		success.put("data", data); 
		return success;
	}
	
	public static Map<String, Object> requestParamError() {
		return ResultMapUtils.errorWithCode(ChatErrorCode.CODE_BAD_REQUEST, 
				ChatErrorCode.DESC_BAD_REQUEST);
	}
	
	public static Map<String, Object> errorWithCode(Integer code, String message) {
		Map<String, Object> map = new HashMap<String, Object>();

		map.put("apicode", code);
		map.put("message", message); 
		return map;
	}
	 
	
	public static Map<String, Object> commonError(String message) {
		Map<String, Object> map = new HashMap<String, Object>();

		map.put("apicode", ChatErrorCode.CODE_COMMON_ERROR);
		map.put("message", message); 
		return map;
	} 
	
	public static Map<String, Object> authorFailed() {
		Map<String, Object> map = new HashMap<String, Object>();

		map.put("apicode", ChatErrorCode.CODE_NO_RIGHT);
		map.put("message", ChatErrorCode.DESC_NO_RIGHT); 
		return map;
	} 
	
	public static Map<String, Object> systemError(){
		Map<String, Object> error = new HashMap<String, Object>();
		error.put("apicode", ChatErrorCode.CODE_SYSTEM_ERROR);
		error.put("message", 
				ChatErrorCode.DESC_SYSTEM_ERROR);
		return error;
	}
	
	public static Map<String, Object> httpSessionTimeoutError(){
		Map<String, Object> error = new HashMap<String, Object>();
		error.put("apicode", ChatErrorCode.CODE_HTTP_SESSION_TIMEOUT);
		error.put("message", ChatErrorCode.DESC_HTTP_SESSION_TIMEOUT);
		return error;
	} 
	
	public static JSONObject authorFailedJson(){
		
		JSONObject json = new JSONObject();
		json.put("apicode", ChatErrorCode.CODE_NO_RIGHT);
		json.put("message", ChatErrorCode.DESC_NO_RIGHT);
		
		return json;
	}
}
