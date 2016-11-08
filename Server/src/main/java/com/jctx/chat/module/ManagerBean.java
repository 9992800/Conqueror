package com.jctx.chat.module;

public class ManagerBean {
	String name;
	String password;
	String srcIp;
	String loginTime;
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getSrcIp() {
		return srcIp;
	}
	public void setSrcIp(String srcIp) {
		this.srcIp = srcIp;
	}
	public String getLoginTime() {
		return loginTime;
	}
	public void setLoginTime(String loginTime) {
		this.loginTime = loginTime;
	}
	@Override
	public String toString() {
		return "ManagerBean [name=" + name + ", password=" + password + ", srcIp=" + srcIp + ", loginTime=" + loginTime
				+ "]";
	}
}
