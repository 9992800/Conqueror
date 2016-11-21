package com.bellflower.conqueror.controller;

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

import com.bellflower.conqueror.Utils.ResultMapUtils;
import com.bellflower.conqueror.module.BattleFields;
import com.bellflower.conqueror.service.BattleFiledService;
import com.bellflower.conqueror.service.ServerListService; 

@Controller
@RequestMapping(value = "/serverHome")
public class HomeController {
	
	@Resource
	ServerListService serverListService; 
	
	@Resource BattleFiledService battleFiledService;
	
	private static final Logger logger = 
			LoggerFactory.getLogger(HomeController.class);

	@RequestMapping(value = "/loginByFacebook")
	@ResponseBody
	public Map<String, Object> loginByFacebook(HttpServletRequest request,
			@RequestParam(value = "user_id", required = true) String user_id) {	
		logger.info("loginByFacebook:" + user_id);
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value = "/battleMaps")
	@ResponseBody
	public Map<String, Object> battleMaps(HttpServletRequest request,
			@RequestParam(value = "user_id", required = true) String user_id,
			@RequestParam(value = "table_id", required = true) String table_id) {		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value = "/createBattle")
	@ResponseBody
	public Map<String, Object> createBattle(HttpServletRequest request,
			@RequestParam(value = "user_id", required = true) String user_id,
			@RequestParam(value = "cell_data", required = true) String cell_data) {
		
		logger.info("createBattle:" + user_id+"===cell_data:"+cell_data);
		
		String idString = battleFiledService.createBattle(user_id, cell_data);
		
 		return ResultMapUtils.success(idString);
	}
	
	@RequestMapping(value = "/battleFields")
	@ResponseBody
	public Map<String, Object> battleFields(HttpServletRequest request,
			@RequestParam(value = "user_id", required = true) String user_id,
			@RequestParam(value = "curPgaeNo", required = true) int curPgaeNo) {	
		List<BattleFields> data = battleFiledService.listBattles(user_id, curPgaeNo);
		return ResultMapUtils.success(data);
	}
	
	@RequestMapping(value = "/players")
	@ResponseBody
	public Map<String, Object> playerList(HttpServletRequest request,
			@RequestParam(value = "user_id", required = true) String user_id,
			@RequestParam(value = "table_id", required = true) String table_id) {		
		return ResultMapUtils.success();
	}
}
