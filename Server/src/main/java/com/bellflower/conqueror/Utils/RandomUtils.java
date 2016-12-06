package com.bellflower.conqueror.Utils;

import java.util.Random;

public final class RandomUtils {
	private static final int VALID_CODE_LEN = 6;
	static Random randGen = new Random();
	static char[] numbersAndLetters = ("0123456789").toCharArray();
	
	public static final String randomNum(int length) {		 
		char[] randBuffer = new char[length];
		for (int i = 0; i < randBuffer.length; i++) {
			randBuffer[i] = numbersAndLetters[randGen.nextInt(10)];
		}
		
		return new String(randBuffer);
	}
	
	public static final int randomNum2(int length) {		 
		return randGen.nextInt(length);
	}
	
	public static final String validCodeForClientApply(){
		return RandomUtils.randomNum(VALID_CODE_LEN);
	}
}
