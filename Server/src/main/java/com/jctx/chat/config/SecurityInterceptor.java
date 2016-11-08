package com.jctx.chat.config;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONObject;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;

import com.jctx.chat.Utils.ResultMapUtils;
import com.jctx.chat.module.JccMsgBean;

public class SecurityInterceptor implements HandlerInterceptor { 

	@Override
	public boolean preHandle(HttpServletRequest request, 
			HttpServletResponse response, Object handler)
			throws Exception {
		
		String requestUri = request.getRequestURI(); 
		
		if (requestUri.startsWith("/jccapi")){
			
			return this.CheckJccAuthor(request, response);
			
		}else if (requestUri.startsWith("/manager")){
			
			return this.checManangerAuthor(request, response);
			
		}else{
			
			return this.checkChatUserAuthor(request, response);
			
		} 
	}

	@Override
	public void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler,
			ModelAndView modelAndView) throws Exception {
	}

	@Override
	public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex)
			throws Exception {
	}
	

	
	
	private void tipsForUnauthorUsers(HttpServletResponse response){
		JSONObject json = ResultMapUtils.authorFailedJson();	
		response.setContentType("application/json;charset=UTF-8");
		
		try {
			
			response.getWriter().write(json.toString());
			response.getWriter().flush();
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private boolean CheckJccAuthor(HttpServletRequest request, HttpServletResponse response) {
		
		String token = request.getParameter("token");
		if (JccMsgBean.canPass(token)){
			return true;
		} 
		
		this.tipsForUnauthorUsers(response);
		
		return false;
	}
	


	private boolean checkChatUserAuthor(HttpServletRequest request, HttpServletResponse response) {
		return true;
	}

	private boolean checManangerAuthor(HttpServletRequest request, HttpServletResponse response) {
		return true;
	}

}
