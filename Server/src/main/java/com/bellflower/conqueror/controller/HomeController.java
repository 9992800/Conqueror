package com.bellflower.conqueror.controller;

import java.util.Map;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import com.bellflower.conqueror.Utils.ResultMapUtils;
import com.bellflower.conqueror.service.ServerListService; 

@Controller
@RequestMapping(value = "/serverHome")
public class HomeController {
	
	@Resource
	ServerListService serverListService;
	
	@Autowired MongoTemplate mongoTemplate;
	
	private static final Logger logger = 
			LoggerFactory.getLogger(HomeController.class);

	@RequestMapping(value = "/loginByFacebook")
	@ResponseBody
	public Map<String, Object> loginByFacebook(HttpServletRequest request,
			@RequestParam(value = "user_id", required = true) String user_id) {	
		logger.info("loginByFacebook:" + user_id);
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value = "/servers")
	@ResponseBody
	public Map<String, Object> serverList(HttpServletRequest request,
			@RequestParam(value = "user_id", required = true) String user_id) {		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value = "/tables")
	@ResponseBody
	public Map<String, Object> tableList(HttpServletRequest request,
			@RequestParam(value = "server_id", required = true) String server_id,
			@RequestParam(value = "user_id", required = true) String user_id) {		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value = "/players")
	@ResponseBody
	public Map<String, Object> playerList(HttpServletRequest request,
			@RequestParam(value = "user_id", required = true) String user_id,
			@RequestParam(value = "table_id", required = true) String table_id) {		
		return ResultMapUtils.success();
	}
}
