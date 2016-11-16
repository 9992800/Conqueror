package com.jctx.chat.controller;

import java.io.IOException;
import java.text.ParseException;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;

import com.jctx.chat.Utils.ResultMapUtils;
import com.jctx.chat.module.User;
import com.jctx.chat.service.FindComponentsService; 

@Controller
public class HomeController {
	
	@Resource
	FindComponentsService findComponentService;
	
	@Autowired MongoTemplate mongoTemplate;
	
	private static final Logger logger = LoggerFactory.getLogger(HomeController.class);

	@RequestMapping(value="/login_open")
	public ModelAndView login_open(HttpServletRequest request, HttpServletResponse response) throws IOException{
		request.getSession();
		return new ModelAndView("login");
	}

	@RequestMapping(value = "/login")
	@ResponseBody
	public Map<String, Object> login(HttpServletRequest request,
			@RequestParam(value = "account", required = true) String phone) {		
		if (!mongoTemplate.collectionExists(User.class)) 
		{ mongoTemplate.createCollection(User.class); } 
		User user = new User();
		user.setName("sasdf");
		user.setAge(12);
		user.setPassword("sdfasd");
		mongoTemplate.insert(user);
		List<User> results = mongoTemplate.findAll(User.class); 
		logger.info("Total amount of persons: {}", results.size());
		logger.info("Results: {}", results); 
		return ResultMapUtils.success(results);
	}
}
