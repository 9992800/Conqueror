package com.jctx.chat.module;

public class MessageBean {
	
	public static final String SYSTEM_CHANNEL = "SYS_40010086";
	/**
	 * 公司运营人员监听的频道
	 */
	public static final String ADMIN_CHANNEL = "ADMIN_40010010";
	public static final long SYSTEM_MSG_TIMEOUT = 7 * 24 * 60 * 60;
	
	public static final int DEVICE_TYPE_IOS     = 1;
	public static final int DEVICE_TYPE_ANDROID = 2;
	public static final int DEVICE_TYPE_WEB = 3;
	
	public static final int CHANNEL_TYPE_PRIVATE = 1;
	public static final int CHANNEL_TYPE_GROUP   = 2;
	public static final int CHANNEL_TYPE_SYSTEM  = 3;
	public static final int CHANNEL_TYPE_ADMIN = 4;
	
	public static final String MESSAGE_TYPE_UNREAD = "unread";
	public static final String MESSAGE_TYPE_LIVE = "live";
	
	private long msgId;
	
	public long getMsgId() {
		return msgId;
	}

	public void setMsgId(long msgId) {
		this.msgId = msgId;
	}

	private String content;
	private int createTime, type, channelType,  from, to;
	
	private Boolean isPingPong = false;

	public MessageBean() {
	}

	public int getCreateTime() {
		return createTime;
	}

	public void setCreateTime(int createTime) {
		this.createTime = createTime;
	}

	public int getFrom() {
		return from;
	}

	public void setFrom(int from) {
		this.from = from;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public int getChannelType() {
		return channelType;
	}

	public void setChannelType(int channelType) {
		this.channelType = channelType;
	}

	public String getContent() {
		return content;
	}

	public void setContent(String content) {
		this.content = content;
	}

	public int getTo() {
		return to;
	}

	public void setTo(int to) {
		this.to = to;
	}

	public Boolean getIsPingPong() {
		return isPingPong;
	}

	public void setIsPingPong(Boolean isPingPong) {
		this.isPingPong = isPingPong;
	}

	@Override
	public String toString() {
		return "MessageBean [msgId=" + msgId + ", createTime=" + createTime + ", from=" + from + ", type=" + type
				+ ", channelType=" + channelType + ", content=" + content + ", to=" + to + "]";
	} 

}
