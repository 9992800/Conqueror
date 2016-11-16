package com.bellflower.conqueror.module;

public class GamePlayer {
	String id;
	String fbId;
	String name;
	String headImg;
	int	   grade;
	int    coinsNum;
	int	   diamonsNum;
	
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getFbId() {
		return fbId;
	}
	public void setFbId(String fbId) {
		this.fbId = fbId;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getHeadImg() {
		return headImg;
	}
	public void setHeadImg(String headImg) {
		this.headImg = headImg;
	}
	public int getGrade() {
		return grade;
	}
	public void setGrade(int grade) {
		this.grade = grade;
	}
	public int getCoinsNum() {
		return coinsNum;
	}
	public void setCoinsNum(int coinsNum) {
		this.coinsNum = coinsNum;
	}
	public int getDiamonsNum() {
		return diamonsNum;
	}
	public void setDiamonsNum(int diamonsNum) {
		this.diamonsNum = diamonsNum;
	}
	
	
	@Override
	public String toString() {
		return "GamePlayer [id=" + id + ", fbId=" + fbId + ", name=" + name + ", headImg=" + headImg + ", grade="
				+ grade + ", coinsNum=" + coinsNum + ", diamonsNum=" + diamonsNum + "]";
	}
}
