package com.bellflower.conqueror.service;

import java.util.List;

import javax.annotation.Resource;

import org.json.JSONArray;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.stereotype.Service;

import com.bellflower.conqueror.module.BattleFieldsBasic;
import com.bellflower.conqueror.module.BattleFieldsMap;
import com.mongodb.WriteResult;
 

@Service
public class BattleFiledService {
	private static final Logger logger = 
			LoggerFactory.getLogger(BattleFiledService.class);
	@Resource
	MongoTemplate mongoService;
	
	public String createBattle(String user_id,String user_name,  JSONArray cells) {
		
		Query query = new Query(Criteria.where("owner").is(user_id));
		WriteResult result = this.mongoService.remove(query, BattleFieldsBasic.class);
		logger.info("battles  basic need to delete: " + result.getN()); 
		
		Query query2 = new Query(Criteria.where("userId").is(user_id));
		WriteResult result2 = this.mongoService.remove(query2, BattleFieldsMap.class);
		logger.info("battles maps need to delete: " + result2.getN());
		
		
		
		BattleFieldsBasic fields = new BattleFieldsBasic();
		fields.setOwner(user_id);
		fields.setOwnerName(user_name);
		mongoService.insert(fields);
		
		
		BattleFieldsMap  maps = new BattleFieldsMap();
		maps.setDataInfo(cells); 
		maps.setId(fields.getId());
		maps.setUserId(user_id);
		mongoService.insert(maps); 
		
		List<BattleFieldsBasic> people =  mongoService.findAll(BattleFieldsBasic.class);
		logger.info("Number of people = : " + people.size());
		
		return fields.getId();
	}

	public List<BattleFieldsBasic> listBattles(String user_id, int start_page) {
		final Pageable pageableRequest = new PageRequest(start_page, 60);
		Query query = new Query();
		query.with(new Sort(Sort.Direction.DESC, "createTime")).with(pageableRequest);
		List<BattleFieldsBasic> rest = mongoService.find(query, BattleFieldsBasic.class);
		return rest;
	}
}
