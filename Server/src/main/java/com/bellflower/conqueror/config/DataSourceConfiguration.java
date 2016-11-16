package com.bellflower.conqueror.config;

import java.beans.PropertyVetoException;

import javax.sql.DataSource;

import org.mybatis.spring.SqlSessionFactoryBean;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.core.io.support.ResourcePatternResolver;

import com.mchange.v2.c3p0.ComboPooledDataSource;

//@Configuration
@PropertySource("classpath:system.properties") 
public class DataSourceConfiguration {
	private static final Logger logger =
		    LoggerFactory.getLogger(DataSourceConfiguration.class);
	
	@Value("${jdbc.url}")
	private String jdbcUrl;
	
	@Value("${jdbc.username}")
	private String userName;
	
	@Value("${jdbc.password}")
	private String passWord;
	
	@Value("${jdbc.driver}")
	private String driver;
	
	@Bean(name = "dataSource")
	public DataSource dataSource(){
		logger.warn("-----------------driver="+driver+"  jdbcUrl=" + jdbcUrl
				+"  userName=" + userName+"  passWord=" + passWord);
		try {
			ComboPooledDataSource dataSource = new ComboPooledDataSource();
			
			dataSource.setDriverClass(driver);
			dataSource.setJdbcUrl(jdbcUrl);
			dataSource.setUser(userName);
			dataSource.setPassword(passWord);
			dataSource.setMinPoolSize(20);
			dataSource.setMaxPoolSize(150);
			dataSource.setMaxIdleTime(1800);
			dataSource.setAcquireIncrement(5);
			dataSource.setMaxStatements(0);
			dataSource.setInitialPoolSize(20);
			dataSource.setIdleConnectionTestPeriod(1800);
			dataSource.setAcquireRetryAttempts(30);
			dataSource.setAcquireRetryDelay(100);
			dataSource.setBreakAfterAcquireFailure(false);
			dataSource.setTestConnectionOnCheckout(false);
		
			return dataSource;
			
		} catch (PropertyVetoException e) { 
			e.printStackTrace();
		}
		
		return null;
	}
	
	@Bean
	public SqlSessionFactoryBean factoryBean() throws Exception{
		SqlSessionFactoryBean factoryBean = new SqlSessionFactoryBean();
		factoryBean.setDataSource(dataSource()); 
		
		PathMatchingResourcePatternResolver pathMatchingResourcePatternResolver = new PathMatchingResourcePatternResolver();        
        String packageSearchPath = ResourcePatternResolver.CLASSPATH_ALL_URL_PREFIX + "/mapper/**/*.xml";
        factoryBean.setMapperLocations(pathMatchingResourcePatternResolver.getResources(packageSearchPath));
        
		return factoryBean;
	} 
}
