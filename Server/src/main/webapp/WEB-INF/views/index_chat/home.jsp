<%@page contentType="text/html"%>
<%@page pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
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

	<title></title>
	<link href="${pageContext.request.contextPath}/css/main/main.css" rel="stylesheet" type="text/css" />
	<link id="easyuiTheme" rel="stylesheet" href="${pageContext.request.contextPath}/js/jquery-easyui-1.3.6/themes/default/easyui.css" type="text/css"></link>
	<link href="${pageContext.request.contextPath}/js/jquery-easyui-1.3.6/themes/icon.css" rel="stylesheet" type="text/css" />
	<link href="${pageContext.request.contextPath}/css/buttons.css" rel="stylesheet" type="text/css" />	
	
	<script type="text/javascript" src="${pageContext.request.contextPath}/js/jquery-easyui-1.3.6/jquery.min.js"></script>
	<script type="text/javascript" src="${pageContext.request.contextPath}/js/jquery-easyui-1.3.6/jquery.easyui.min.js"></script>
	<script type="text/javascript" src="${pageContext.request.contextPath}/js/jquery-easyui-1.3.6/locale/easyui-lang-zh_CN.js"></script>
	
	<script type="text/javascript" src="${pageContext.request.contextPath}/js/main/main.js"></script>
</head>
<body class="easyui-layout">
	<div data-options="region:'north',split:true,border:false" style="height:50px;overflow: hidden;background-image: url('${pageContext.request.contextPath}/images/index_north.jpg');">
		<%@include file="north.jsp"%>
	</div>
	
	<!-- ,collapsed:true 展开关闭 -->
	<div data-options="region:'west',title:'导航菜单',split:true" style="width:200px;">
		<%@include file="west.jsp"%>
	</div>

	<div data-options="region:'center',title:'欢迎您: 【${sessionScope.boss.organizerName}】'" style="background:#eee;overflow: hidden;">
		<%@include file="center.jsp"%>
	</div>
	
	<div data-options="region:'south',split:false" style="background-color:#EEEEEE;"></div>
</body>
</html>