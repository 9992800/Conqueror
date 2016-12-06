package com.bellflower.conqueror.module;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.bellflower.conqueror.Utils.RandomUtils;



public class RandomMap {

	public final static int  DIR_INAREA = 6;
	public final static int  XMAX = 32;
	public final static int  YMAX = 28;
	public final static int  CEL_MAX = (XMAX * YMAX);
	public final static int  PUT_DICE =(3);
	public final static int  STOCK_MAX = (64);
	public final static int  AREA_MAX =(32);
	public final static int  MAX_PLAYER =(8);
	public final static int  MAX_DICE_PER_AREA =(8); 
	
	public final static int CELL_DIR_RIGHT_TOP = 0;
	public final static int CELL_DIR_RIGHT_MID = 1;
	public final static int CELL_DIR_RIGHT_BOT = 2;
	public final static int CELL_DIR_LEFT_TOP = 3;
	public final static int CELL_DIR_LEFT_MID = 4; 
	public final static int CELL_DIR_LEFT_BOT = 5;
	
	private static final Logger logger = 
			LoggerFactory.getLogger(RandomMap.class); 	
	private JSONArray cellData = new JSONArray();	
	public RandomMap(int size) {
		this.createMapData();
	}
	
	void createMapData(){ 
		List<List<Integer>> _join = new ArrayList<List<Integer>>(CEL_MAX);
		for (int i = 0; i < CEL_MAX; i++){
			List<Integer> _dir = new ArrayList<Integer>(DIR_INAREA);
			for (int j = 0; j < DIR_INAREA; j++){
				int value = this.next_cel(i, j);
				_dir.set(j, value);
			}
			_join.set(i, _dir);
		}
		
		int _num[]= new int[CEL_MAX];
		for (int i = 0; i < CEL_MAX; i++){
			_num[i] = i;
		}
		
		for (int i = 0; i < CEL_MAX; i++){
			int random = RandomUtils.randomNum2(CEL_MAX);
			int tmp = _num[i];
			_num[i] = _num[random];
			_num[random] = tmp;  
		}
		
		int _cel[]= new int[CEL_MAX];
		int _rcel[] = new int[CEL_MAX];
		int r = RandomUtils.randomNum2(CEL_MAX);
		_rcel[r] = 1;
		
		for (int i = 1; i < AREA_MAX; i++){
			int valaible_cel  = 9999;
            int selected_cell = 0;
            for (int j = 0; j < CEL_MAX; j++) {
                    if (_cel[j] <= 0
                        && _num[j] <= valaible_cel
                        &&_rcel[j] != 0){
                            valaible_cel = _num[j];
                            selected_cell = j;
                    }
            }
            
            if (9999 == valaible_cel){
                    break;
            }
            
            
            int next_f[] = new int[CEL_MAX]; 
            int cell_num_in_area = 0;
            int cell_in_area = selected_cell;
            do{
                    _cel[cell_in_area] = i;
                    ++cell_num_in_area;
                    
                    for (int k = 0; k < DIR_INAREA; k++){
                            int joined_cell = _join.get(cell_in_area).get(k);
                            if (joined_cell >= 0){
                                    next_f[joined_cell] = 1;
                            }
                    }
                    
                    int cell_value = 9999;
                    for (int k = 0; k < CEL_MAX; k++){
                            if (next_f[k] != 0
                                && _cel[k] <= 0
                                && _num[k] <= cell_value){
                                    cell_value = _num[k];
                                    cell_in_area = k;
                            }
                    }
                    if (9999 == cell_value){
                            break;
                    }
            }while(cell_num_in_area < 8);
            
            for (int j = 0; j < CEL_MAX; j++){
                    
                    if (next_f[j] != 0 && _cel[j] <= 0){
                            
                            _cel[j] = i;
                            ++cell_num_in_area;
                            
                            for (int k = 0; k < DIR_INAREA; k++){
                                    int joined_cell = _join.get(j).get(k);
                                    if (joined_cell >= 0){
                                            _rcel[joined_cell] = 1;
                                    }
                            }
                    }
            }
            
            if (0 ==  cell_num_in_area){
                    break;
            }
		}
		
		for (int i = 0; i < CEL_MAX; i++){
            if (_cel[i] > 0){
                    continue;
            }
            
            int areaIdx = 0;
            boolean areaNotUsed = false;
            for (int j = 0; j < DIR_INAREA; j++) {
                    int joined_cell = _join.get(i).get(j);
                    if (joined_cell < 0){
                            continue;
                    }
                    
                    if (_cel[joined_cell] == 0){
                            areaNotUsed = true;
                    }else{
                            areaIdx = _cel[joined_cell];
                    }
            }
            
            if (!areaNotUsed){
                    _cel[i] = areaIdx;
            }
		}
		
		int _area_size[] = new int[AREA_MAX];
        for (int i = 0; i < CEL_MAX; i++){
                if (_cel[i] > 0)
                        _area_size[_cel[i]]++;
        }
        
        for (int i = 1; i < AREA_MAX; i++){
                if (_area_size[i] <= 5){
                        _area_size[i] = 0;
                }
        }
        
        for (int i = 0; i < CEL_MAX; i++){
                if (i % AREA_MAX == 0){
                	logger.info("\r\n");
                }
                logger.info("," + _cel[i]);
                this.cellData.put(_cel[i]);
        }
	}
	
	public JSONArray getCellData() {
		return cellData;
	}
	
	public void setCellData(JSONArray cellData) {
		this.cellData = cellData;
	}
	
	int next_cel(int opos, int dir){
		int column = opos % XMAX;
		int rows = opos / XMAX;
		
		int odd_rows = rows % 2;
		int iFlag = 0, jFlag = 0;
		switch (dir) {
		case CELL_DIR_RIGHT_TOP:
            iFlag = odd_rows == 1 ? 1: 0;
            jFlag = -1;
			break;
        case CELL_DIR_RIGHT_MID:
            iFlag = 1;
            break;
    case CELL_DIR_RIGHT_BOT:
            iFlag = odd_rows == 1 ? 1 : 0;
            jFlag = 1;
            break;
    case CELL_DIR_LEFT_TOP:
            iFlag = odd_rows == 1 ? 0 : -1;
            jFlag = 1;
            break;
    case CELL_DIR_LEFT_MID:
            iFlag = -1;
            break;
    case CELL_DIR_LEFT_BOT:
            iFlag = odd_rows == 1 ? 0 : -1;
            jFlag = -1;
            break;
		}
		
		column += iFlag;
        rows +=jFlag;        
        if (column < 0 || column >= XMAX
            || rows < 0 ||rows >= YMAX){
                return -1;
        }
        
		return rows * XMAX + column;
	}
	
	void initDir(){
		
	}
}
