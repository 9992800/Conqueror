package com.bellflower.conqueror.service;

import java.util.List;

import javax.annotation.Resource;

import org.json.JSONArray;
import org.json.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.stereotype.Service;

import com.bellflower.conqueror.module.BattleFields;
import com.mongodb.WriteResult;
 

@Service
public class BattleFiledService {
	private static final Logger logger = 
			LoggerFactory.getLogger(BattleFiledService.class);
	@Resource
	MongoTemplate mongoService;
	
	public String createBattle(String user_id, String cell_data) {
		
//		Query query = new Query(Criteria.where("owner").is(user_id));
//		WriteResult result = this.mongoService.remove(query, BattleFields.class);
//		logger.info("battles need to delete: " + result.getN()); 
		
		JSONObject cell_json = new JSONObject(cell_data);
		JSONArray cells = cell_json.getJSONArray("_cells");
		
		BattleFields fields = new BattleFields();
		fields.setOwner(user_id);
		fields.setDataInfo(cells); 
		
		mongoService.insert(fields);
		
		List<BattleFields> people =  mongoService.findAll(BattleFields.class);
		logger.info("Number of people = : " + people.size());
		
		return fields.getId();
	}

	public List<BattleFields> listBattles(String user_id, int start_page) {
		final Pageable pageableRequest = new PageRequest(start_page, 60);
		Query query = new Query();
		query.with(new Sort(Sort.Direction.DESC, "createTime")).with(pageableRequest);
		List<BattleFields> rest = mongoService.find(query, BattleFields.class);
		return rest;
	}
}
