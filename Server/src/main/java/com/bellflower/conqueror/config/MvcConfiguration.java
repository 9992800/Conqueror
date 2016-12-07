package com.bellflower.conqueror.config;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.PropertySource;
import org.springframework.context.support.PropertySourcesPlaceholderConfigurer;
import org.springframework.core.convert.ConversionService;
import org.springframework.format.support.DefaultFormattingConversionService;
import org.springframework.scheduling.annotation.EnableScheduling;
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;
import org.springframework.web.servlet.ViewResolver;
import org.springframework.web.servlet.config.annotation.EnableWebMvc;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.ResourceHandlerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;
import org.springframework.web.servlet.view.InternalResourceViewResolver;

@Configuration
@ComponentScan(basePackages="com.bellflower.conqueror")
@EnableWebMvc
@EnableScheduling
@PropertySource("classpath:system.properties")
public class MvcConfiguration extends WebMvcConfigurerAdapter{

	@Bean
	public ViewResolver getViewResolver(){
		InternalResourceViewResolver resolver = new InternalResourceViewResolver();
		resolver.setPrefix("/WEB-INF/views/");
		resolver.setSuffix(".jsp");
		return resolver;
	}
	
	@Override
	public void addResourceHandlers(ResourceHandlerRegistry registry) {
		registry.addResourceHandler("/**/*.html","/**/*.js","/**/*.css",
				"/**/*.png","/**/*.jpg","/**/*.ico","/**/*.gif")
		.addResourceLocations("/");
	}
	
	@Override
	public void addInterceptors(InterceptorRegistry registry) {
		registry.addInterceptor(sercurityInterceptor());
		registry.addInterceptor(sessionInterceptor());
	}
	
	@Bean
	SecurityInterceptor sercurityInterceptor(){
		return new SecurityInterceptor();
	}
	
	@Bean
	SessionInterceptor sessionInterceptor(){
		return new SessionInterceptor();
	}

	@Bean
    public static PropertySourcesPlaceholderConfigurer placeholderConfigurer() {
        return new PropertySourcesPlaceholderConfigurer();
    }
	
	@Bean
	public static ConversionService conversionService() {
	    return new DefaultFormattingConversionService();
	}
	
	@Value("${thread.pool.coresize}")
	private int __coreThreadPoolSize;
	
	@Value("${thread.pool.maxsize}")
	private int __coreThreadMaxSize;
	
	@Value("${thread.pool.queuesize}")
	private int __queueCapacity;
	
	@Bean
	public ThreadPoolTaskExecutor taskExecutor() {
		ThreadPoolTaskExecutor pool = new ThreadPoolTaskExecutor();
		pool.setCorePoolSize(__coreThreadPoolSize);
		pool.setMaxPoolSize(__coreThreadMaxSize);
		pool.setQueueCapacity(__queueCapacity);
		pool.setWaitForTasksToCompleteOnShutdown(true);
		return pool;
	}
}
