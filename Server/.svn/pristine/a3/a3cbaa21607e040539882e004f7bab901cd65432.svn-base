package com.jctx.chat.controller;

import java.io.IOException;
import java.util.Map;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import com.jctx.chat.Utils.ResultMapUtils;
import com.jctx.chat.module.ChatUserBean;
import com.jctx.chat.service.UserManagerService;

@Controller
@RequestMapping(value="/chatUser")
public class UserManagerController {
	
	private static final Logger logger =
		    LoggerFactory.getLogger(UserManagerController.class);
	
@Resource UserManagerService userManagerService;
	
	@RequestMapping(value="/getChatUserInfo")
	@ResponseBody
	public Map<String, Object> getChatUserInfo(HttpServletRequest request, 
			@RequestParam(value = "uid", required = true) int uid) throws IOException{
		
		logger.info("---getChatUserInfo:---" + uid);
		
		ChatUserBean bean = userManagerService.getChatUserInfo(uid);
		
		return ResultMapUtils.success(bean);
	}
	
	@RequestMapping(value="/createChatUser")
	@ResponseBody
	public Map<String, Object> createChatUser(HttpServletRequest request, 
			ChatUserBean bean) throws IOException{
		
		logger.info("---createChatUser:---" + bean.toString());
		
		userManagerService.createChatUser(bean);
		
		return ResultMapUtils.success(bean);
	}
	
	@RequestMapping(value="/updateChatUser")
	@ResponseBody
	public Map<String, Object> updateChatUser(HttpServletRequest request, 
			ChatUserBean bean) throws IOException{
		
		logger.info("---updateChatUser:---" + bean.toString());
		
		userManagerService.updateChatUser(bean);
		
		return ResultMapUtils.success();
	}
}
