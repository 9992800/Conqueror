package com.jctx.chat.controller;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import com.jctx.chat.Utils.ResultMapUtils;
import com.jctx.chat.module.ChatGroupBean;
import com.jctx.chat.module.ChatUserBean;
import com.jctx.chat.module.MessageBean;
import com.jctx.chat.service.ChatDbService;
import com.jctx.chat.service.ChatGroupService;
import com.jctx.chat.service.UserManagerService;

@Controller
@RequestMapping(value="/manager")
public class SystemManagerController {
	
	private static final Logger logger =
		    LoggerFactory.getLogger(SystemManagerController.class);
	
	@Resource
	ChatDbService chatDbService;
	
	@Resource UserManagerService userManagerService;
	
	@Resource ChatGroupService chatGroupService;
	
	@RequestMapping(value="/flushMessage")
	@ResponseBody
	public Map<String, Object> flushMessage(HttpServletRequest request,
			HttpServletResponse response) throws IOException{
		
		logger.info("---flush current chat message---");
		
		int count = chatDbService.flushMsgToDb();
		
		return ResultMapUtils.success(count);
	}
	
	@RequestMapping(value="/listMessage")
	@ResponseBody
	public Map<String, Object> listMessage(HttpServletRequest request,
			HttpServletResponse response) throws IOException{
		
		logger.info("---list current chat message---");
		
		ArrayList<MessageBean> list = chatDbService.getCurrentQueue();
		
		return ResultMapUtils.success(list);
	}
	
	@RequestMapping(value="/listUsers")
	@ResponseBody
	public Map<String, Object> listUsers(HttpServletRequest request,
			@RequestParam(value = "startUid", required = true) int startUid) throws IOException{
		 
		logger.info("---list system users---");
		
		List<ChatUserBean> users = userManagerService.getChatUsers(startUid);
		
		return ResultMapUtils.success(users);
	}
	
	@RequestMapping(value="/listOnlineUsers")
	@ResponseBody
	public Map<String, Object> listOnlineUsers(HttpServletRequest request) throws IOException{ 
		
		logger.info("---list current onlin users---");
		
		List<String> users = userManagerService.getOnlineChatUsers();
		
		return ResultMapUtils.success(users);
	}
	
	@RequestMapping(value="/listGroups")
	@ResponseBody
	public Map<String, Object> listGroups(HttpServletRequest request,
			@RequestParam(value = "startGrpId", required = true) int startGrpId) throws IOException{
		
		logger.info("---list current chat gourps---");
		
		List<ChatGroupBean> grps= chatGroupService.listGroups(startGrpId);
		
		return ResultMapUtils.success(grps);
	}
	
	@RequestMapping(value="/kickoutOneUser")
	@ResponseBody
	public Map<String, Object> kickoutOneUser(HttpServletRequest request,
			HttpServletResponse response) throws IOException{
		
		logger.info("---kickout some duplicate users---");
		
		return ResultMapUtils.success();
	}
}
