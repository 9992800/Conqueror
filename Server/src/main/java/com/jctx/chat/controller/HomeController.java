package com.jctx.chat.controller;

import java.io.IOException;
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
import org.springframework.web.servlet.ModelAndView;

import com.jctx.chat.Utils.ResultMapUtils;
import com.jctx.chat.service.FindComponentsService; 

@Controller
public class HomeController {
	
	@Resource
	FindComponentsService findComponentService;
	
	private static final Logger logger = LoggerFactory.getLogger(HomeController.class);

	@RequestMapping(value="/login_open")
	public ModelAndView login_open(HttpServletRequest request, HttpServletResponse response) throws IOException{
		request.getSession();
		return new ModelAndView("login");
	}

	@RequestMapping(value = "/login")
	@ResponseBody
	public Map<String, Object> login(HttpServletRequest request,
			@RequestParam(value = "account", required = true) String phone,
			@RequestParam(value = "password", required = true) String passwd) {
		request.getSession();
		
		/*if(PasswordUtil.verify("lws502",manager.getPassword())){
			return new ModelAndView("main");
		}*/
		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value="/home")
	public ModelAndView home(){
		
		return new ModelAndView("index_chat/home");
	}
}
