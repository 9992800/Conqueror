<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<!--[if lt IE 7]> <html class="lt-ie9 lt-ie8 lt-ie7" lang="en"> <![endif]-->
<!--[if IE 7]> <html class="lt-ie9 lt-ie8" lang="en"> <![endif]-->
<!--[if IE 8]> <html class="lt-ie9" lang="en"> <![endif]-->
<!--[if gt IE 8]><!-->
<html>
<!--<![endif]-->
<head>
	<meta charset="utf-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
	
	<!-- web app设置 -->
	<meta name="HandheldFriendly" content="true">
	<meta name="viewport" content="width=device-width,initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no"/>
	<meta name="format-detection" content="telephone=no">
	<!-- 启用 WebApp 全屏模式 -->
	<meta name="apple-mobile-web-app-capable" content="yes" /> 
	<meta name="apple-mobile-web-app-status-bar-style" content="black-translucent" />
	<!-- 针对手持设备优化，主要是针对一些老的不识别viewport的浏览器，比如黑莓 -->
	<meta name="HandheldFriendly" content="true">
	<!-- 微软的老式浏览器 -->
	<meta name="MobileOptimized" content="320">
	<!-- uc强制竖屏 -->
	<meta name="screen-orientation" content="portrait">
	<!-- QQ强制竖屏 -->
	<meta name="x5-orientation" content="portrait">
	<!-- UC强制全屏 -->
	<meta name="full-screen" content="yes">
	<!-- QQ强制全屏 -->
	<meta name="x5-fullscreen" content="true">
	<!-- UC应用模式 -->
	<meta name="browsermode" content="application">
	<!-- QQ应用模式 -->
	<meta name="x5-page-mode" content="app">
	<!-- windows phone 点击无高光 -->
	<meta name="msapplication-tap-highlight" content="no">
	<!-- ################################## -->
	
	<title>后台入口</title>
	<link rel="stylesheet" href="${pageContext.request.contextPath}/css/loginStyle.css">
	<script type="text/javascript" src="${pageContext.request.contextPath}/js/main/jquery-1.8.3.min.js"></script>
	<script type="text/javascript" src="${pageContext.request.contextPath}/js/main/jquery.cookie.js"></script>
	<script type="text/javascript" src="${pageContext.request.contextPath}/js/main/common.js"></script>
</head>
<body>
	<section class="container">
		<div class="login">
			<h1>&nbsp;</h1>
			<p>
				<input type="text" id="account" name="account" value=""  placeholder="账户">
			</p>
			
			<p>
				<input type="password" id="password" name="password" value="" placeholder="密码"> 
			</p>
			
			<p style="text-align: center; display: none;">
				<select id="role" name="role" style="width:96%; height: 34px;">
					<option value="admin" selected="selected">系统管理员</option>
				</select>
			</p>
			
			<p>
				<input type="checkbox" id="rmbUser" style="margin-bottom: 0px;">记住我(1周)
			</p>
			
			<p class="submit" id="submitBtn" >
				<input type="submit" value="登录" onclick="return signin();">
			</p>
			
			<p class="submit" id="submitAlert" style="display: none;">
				<input type="submit" value="正在登录,请稍后...">
			</p>
		</div>
	</section>
</body>
</html>
<script>
	$(function(){
		ifRemember();
	});
	
	function ifRemember(){
		var isRemember = $.cookie("rmbUser");
		if (isRemember) {
	        $("#rmbUser").attr("checked", true);
	        
	        var account = $.cookie("userName");
	        var password = $.cookie("passWord");
	        var role = $.cookie("role");
	        
	        $("#account").val(account);
	        $("#password").val(password);
	        $("#role").val(role);
	    }
	}
	
	function saveUserInfo() {
		var isCheck = $("#rmbUser").attr("checked");
	    if (isCheck == "checked") {
	        var userName = $("#account").val().trim();
	        var passWord = $("#password").val().trim();
	        var role = $("#role").val().trim();
	        
	        $.cookie("rmbUser", "true", { expires: 7 });
	        $.cookie("userName", userName, { expires: 7 });
	        $.cookie("passWord", passWord, { expires: 7 });
	        $.cookie("role", role, { expires: 7 });
	    }else {
	        $.cookie("rmbUser", false, { expires: -1 });
	        $.cookie("userName", '', { expires: -1 });
	        $.cookie("passWord", '', { expires: -1 });
	        $.cookie("role", '', { expires: -1 });
	    }
	}

	function signin(){
		saveUserInfo
		var role = $("#role").val().trim();
		var account = $("#account").val().trim();
		var password = $("#password").val().trim();
		
		if(role == "admin"){
			adminSignin(account,password);
		}
	}	
	
	
	function adminSignin(account_email,password){
		
		showAndHide("submitAlert","submitBtn");
		$.ajax({
			url : "/login",
			data : {
				account : account_email,
				password : password
			},
			type : 'post',
			cache : false,
			dataType : 'json',
			success : function(data) {
				window.location.href = "/home";
				showAndHide("submitBtn","submitAlert");
				return true;					
			},
			error : function() {
				return false;
			}
		});
	}
	
	document.onkeydown = function(event_e){
		if(window.event){
			event_e = window.event;
		}
		var int_keycode = event_e.charCode||event_e.keyCode;
		if(int_keycode == 13){ 
			signin();
		}
	}
</script>

