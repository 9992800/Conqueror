package com.jctx.chat.controller;

import java.io.IOException;
import java.util.Map;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import com.jctx.chat.Utils.ChatErrorCode;
import com.jctx.chat.Utils.ResultMapUtils;
import com.jctx.chat.module.JccMsgBean;
import com.jctx.chat.service.JccApiService;

@Controller
@RequestMapping(value="/jccapi")
public class JccApiController {

	private static final Logger logger =
		    LoggerFactory.getLogger(JccApiController.class);
	
	@Resource JccApiService jccApiService;
	
	@RequestMapping(value="/sysmsg_toprivate")
	@ResponseBody
	public Map<String, Object> systemToPrivate(HttpServletRequest request, JccMsgBean bean) throws IOException{
		
		logger.info("---system to private:---"+bean.toString());
		
		if (!bean.validate()){
			
			ResultMapUtils.errorWithCode(ChatErrorCode.CODE_BAD_REQUEST, 
					ChatErrorCode.DESC_BAD_REQUEST);
		}
		
		jccApiService.fromSysToPrivate(bean);
		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value="/broadcast")
	@ResponseBody
	public Map<String, Object> broadcast(HttpServletRequest request, JccMsgBean bean) throws IOException{
		
		logger.info("---broadcast:---" + bean.toString());
		
		jccApiService.broadcast(bean, true);
		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value="/broadcast_test")
	@ResponseBody
	public Map<String, Object> broadcastTest(HttpServletRequest request, JccMsgBean bean) throws IOException{
		
		logger.info("---broadcastTest:---" + bean.toString());
		
		jccApiService.broadcast(bean, false);
		
		return ResultMapUtils.success();
	}
	
	@RequestMapping(value="/broadcast_toadmin")
	@ResponseBody
	public Map<String, Object> broadcastToAdmin(JccMsgBean bean) {
		try {
			bean.setMessage(new String(bean.getMessage().getBytes("iso8859-1"), "utf-8"));
			jccApiService.broadcastToAdmin(bean);
			return ResultMapUtils.success();
		} catch (Exception e) {
			logger.error("JccApiController broadcastToAdmin error!", e);
		}
		return ResultMapUtils.commonError("process error!");
	}
}
