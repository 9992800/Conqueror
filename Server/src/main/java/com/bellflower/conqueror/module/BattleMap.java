package com.bellflower.conqueror.module;

import java.util.ArrayList;

public class BattleMap {
	ArrayList<Integer> cellData;
	ArrayList<Integer> areaData;
	
	public ArrayList<Integer> getCellData() {
		return cellData;
	}
	public void setCellData(ArrayList<Integer> cellData) {
		this.cellData = cellData;
	}
	public ArrayList<Integer> getAreaData() {
		return areaData;
	}
	public void setAreaData(ArrayList<Integer> areaData) {
		this.areaData = areaData;
	}	
}
