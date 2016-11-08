package com.jctx.chat.controller;

import java.util.List;
import java.util.Map;
import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import com.jctx.chat.Utils.ChatException;
import com.jctx.chat.Utils.ResultMapUtils;
import com.jctx.chat.module.ChatGroupBean;
import com.jctx.chat.module.ChatGroupMemBean;
import com.jctx.chat.module.ChatUserBean;
import com.jctx.chat.service.ChatGroupService;

@Controller
@RequestMapping(value="/group")
public class ChatGroupController {
	private static final Logger logger =
		    LoggerFactory.getLogger(ChatGroupController.class);
	
	@Resource ChatGroupService chatGroupService;
	
	
	@RequestMapping(value="/createGroup")
	@ResponseBody
	public Map<String, Object> createGroup(HttpServletRequest request,
			ChatUserBean bean,
			@RequestParam(value = "groupName", required = true) String groupName){
		logger.info("---creat new group---"+bean.toString());
		
		boolean canCreate = chatGroupService.checkAuthorize(bean);
		if (!canCreate) {
			return ResultMapUtils.commonError("超出创建群组个数或尚未开通群组");
		}
		
		try {
			chatGroupService.createChatGroup(bean, groupName);
		} catch (ChatException e) { 
			e.printStackTrace();
			return e.getWhat();
		}
		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value="/getAllGroup")
	@ResponseBody
	public Map<String, Object> getAllGroup(HttpServletRequest request, ChatUserBean bean){
		logger.info("---list all my groups---"+bean.toString());
		List<ChatGroupBean> list = chatGroupService.getAllGroup(bean.getUid());
		return ResultMapUtils.success(list);
	}
	
	@RequestMapping(value="/getGroupDetails")
	@ResponseBody
	public Map<String, Object> getGroupDetails(HttpServletRequest request, 
			ChatUserBean bean,
			@RequestParam(value = "groupId", required = true) Integer groupId){
		logger.info("---show group details---"+bean.toString());
		
		ChatGroupBean beanGrp = chatGroupService.getGroupDetails(bean.getUid(), groupId);
		
		return ResultMapUtils.success(beanGrp);
	}
	
	@RequestMapping(value="/deleteGroup")
	@ResponseBody
	public Map<String, Object> deleteGroup(HttpServletRequest request, 
			ChatUserBean bean,
			@RequestParam(value = "groupId", required = true) Integer groupId){
		logger.info("---delete new group---"+bean.toString());
		try {
			chatGroupService.deleteGroup(bean.getUid(), groupId);
		} catch (ChatException e) { 
			e.printStackTrace();
			return e.getWhat();
		}
		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value="/addGroupMembers")
	@ResponseBody
	public Map<String, Object> addGroupMembers(HttpServletRequest request, 
			@RequestParam(value = "grpId", required = true) int grpId,
			@RequestParam(value = "userIdList[]", required = true) Integer[] userIdList){
		logger.info("---add  group members---"+grpId+" ids="+userIdList);
		
		try {
			chatGroupService.addNewMembers(grpId, userIdList);
		} catch (ChatException e) { 
			e.printStackTrace();
			return e.getWhat();
		}
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value="/getAllGroupMembers")
	@ResponseBody
	public Map<String, Object> getAllGroupMembers(HttpServletRequest request, ChatUserBean bean,
			@RequestParam(value = "grpId", required = true) int grpId){
		logger.info("---get group members---"+bean.toString());
		boolean canLook = chatGroupService.checkGroupLooking(bean, grpId);
		if (!canLook){
			return ResultMapUtils.authorFailed();
		}
		List<ChatGroupMemBean> list = chatGroupService.getAllGroupMembers(grpId);
		return ResultMapUtils.success(list);
	}
	
	@RequestMapping(value="/delGroupMembers")
	@ResponseBody
	public Map<String, Object> delGroupMembers(HttpServletRequest request, 
			ChatUserBean bean,
			@RequestParam(value = "grpId", required = true) int grpId,
			@RequestParam(value = "toBeDelUids[]", required = true) Integer[] toBeDelUids){
		logger.info("---delete group members---"+bean.toString());
		boolean canLook = chatGroupService.checkGroupLooking(bean, grpId);
		if (!canLook){
			return ResultMapUtils.authorFailed();
		}
		
		chatGroupService.delGroupMembers(grpId, toBeDelUids);
		
		return ResultMapUtils.success();
	}
}
