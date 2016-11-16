package com.bellflower.conqueror.Utils;

import java.io.IOException;

import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class HttpUtils {
	
	private static final Logger log = LoggerFactory.getLogger(HttpUtils.class);
	
	public static String get(String url, String query) {
		CloseableHttpClient httpclient = HttpClients.createDefault();
		try {
			HttpGet httpGet = new HttpGet(url + "?" + query);
			CloseableHttpResponse response = httpclient.execute(httpGet);
			try {
				return EntityUtils.toString(response.getEntity());
			} catch (Exception e) {
				log.error("HttpUtils get error!", e);
			} finally {
				response.close();
			}
		} catch (Exception e) {
			log.error("HttpUtils get error!", e);
		} finally {
			try {
				httpclient.close();
			} catch (IOException e) {
				log.error("HttpUtils get error!", e);
			}
		}
		return null;
	}
}
