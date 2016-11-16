package com.bellflower.conqueror.Utils;

import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CheckUtils {
	
private static final Pattern pattern = Pattern.compile("^1[3-8]{1}\\d{9}$");
	
	private static final Pattern patternYzm = Pattern.compile("^\\d{6}$");
	
	public static Map<String, Object> checkYzm(String yzm) {
		Matcher matcherYzm = patternYzm.matcher(yzm);
		if (yzm == null || !matcherYzm.matches()) {
			return ResultMapUtils.errorWithCode(ChatErrorCode.CODE_COMMON_ERROR, "验证码格式错误");
		}
		return null;
	}
	
	public static Map<String, Object> checkPhone(String phone) {
		Matcher matcher = pattern.matcher(phone);
		if (phone == null || !matcher.matches()) {
			return ResultMapUtils.errorWithCode(ChatErrorCode.CODE_COMMON_ERROR, "请输入正确的电话格式");
		}
		return null;
	}
	
	public static Map<String, Object> checkUsername(String userName){
		if (userName == null || userName.length() < 2 || userName.length() > 10) {
			return ResultMapUtils.errorWithCode(ChatErrorCode.CODE_COMMON_ERROR, "请输入正确格式的姓名");
		}
		return null;
	}
}
