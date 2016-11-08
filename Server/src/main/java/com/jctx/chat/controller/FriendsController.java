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
import com.jctx.chat.module.ChatUserBean;
import com.jctx.chat.module.FriendsApplication;
import com.jctx.chat.module.FriendsRelationShip;
import com.jctx.chat.module.MessageBean;
import com.jctx.chat.service.FriendsService;

@Controller
@RequestMapping(value="/friends")
public class FriendsController {
	
	private static final Logger logger =
		    LoggerFactory.getLogger(FriendsController.class);
	
	@Resource FriendsService friendsService;
	
	@RequestMapping(value="/applyAsFriends") 
	@ResponseBody
	public Map<String, Object> applyAsFriends(HttpServletRequest request,
			ChatUserBean applicant,
			@RequestParam(value = "targetUid", required = true) int targetUid){
		
		logger.info("---apply as friends---"+applicant.toString());
		
		try {
			
			friendsService.applyAsFriends(applicant, targetUid);
			
			return ResultMapUtils.success();
			
		}catch (ChatException e) {
			return e.getWhat();
		}catch (Exception e) {
			e.printStackTrace();
			return ResultMapUtils.systemError();
		} 
	}
	
	@RequestMapping(value="/approvedAsFriends")
	@ResponseBody
	public Map<String, Object> approvedAsFriends(HttpServletRequest request, 
			ChatUserBean bean, 
			@RequestParam(value = "applicantUid", required = true) int applicantUid){
		
		logger.info("---approved as friends---"+bean.toString());
		
		try {
			friendsService.approvedAsFriends(bean, applicantUid);
		} catch (ChatException e) { 
			e.printStackTrace();
			return e.getWhat();
		}
		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value="/applicantList")
	@ResponseBody
	public Map<String, Object> applicantList(HttpServletRequest request, 
			ChatUserBean bean){
		
		logger.info("---list all my friends---"+bean.toString());
		
		List<FriendsApplication> list = friendsService.getAllApplicants(bean);
		
		return ResultMapUtils.success(list);
	}
	
	@RequestMapping(value="/friendsList")
	@ResponseBody
	public Map<String, Object> friendsList(HttpServletRequest request, 
			ChatUserBean bean){
		
		logger.info("---list all my friends---"+bean.toString());
		
		List<FriendsRelationShip> list = friendsService.getAllMyFriends(bean);
		
		return ResultMapUtils.success(list);
	}
	
	@RequestMapping(value="/deleteFriend")
	@ResponseBody
	public Map<String, Object> deleteFriend(HttpServletRequest request,
			ChatUserBean bean,
			@RequestParam(value = "toBeDelUid", required = true) int toBeDelUid){
		logger.info("---delete one of my friends---"+bean.toString());
		
		try {
			friendsService.removeMyFriends(bean, toBeDelUid);
		} catch (ChatException e) { 
			e.printStackTrace();
			return e.getWhat();
		}
		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value="/addToBlack")
	@ResponseBody
	public Map<String, Object> addToBlack(HttpServletRequest request,
			ChatUserBean bean,
			@RequestParam(value = "toBlackUid", required = true) int toBlackUid){
		logger.info("---add one user to black list---"+bean.toString());
		
		try {
			friendsService.blockSomeOne(bean, toBlackUid);
		} catch (ChatException e) { 
			e.printStackTrace();
			return e.getWhat();
		}
		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value="/blackList")
	@ResponseBody
	public Map<String, Object> blackList(HttpServletRequest request, ChatUserBean bean){
		logger.info("---list all users in my black list---"+bean.toString());
		
		List<FriendsRelationShip> list = friendsService.getAllInBlack(bean);
		
		return ResultMapUtils.success(list);
	}
	
	@RequestMapping(value="/removeFromBlack")
	@ResponseBody
	public Map<String, Object> removeFromBlack(HttpServletRequest request,
			ChatUserBean bean,
			@RequestParam(value = "toBeDelUid", required = true) int toBeDelUid){
		logger.info("---remove user from black list---"+bean.toString());
		
		try {
			friendsService.removeFromBlack(bean, toBeDelUid);
		} catch (ChatException e) { 
			e.printStackTrace();
			return e.getWhat();
		}
		
		return ResultMapUtils.success();
	}
	
	
	@RequestMapping(value="/syncChatHistory")
	@ResponseBody
	public Map<String, Object> syncChatHistory(HttpServletRequest request, 
			ChatUserBean bean,
			@RequestParam(value = "peerUid", required = true) int peerUid,
			@RequestParam(value = "startMsgId", required = true) int startMsgId){
		logger.info("---sync chat message history---"+bean.toString());
		
		List<MessageBean> msg_list = friendsService.getHistoryMsgOfFriends(bean, peerUid, startMsgId);
		
		return ResultMapUtils.success(msg_list);
	}
}
