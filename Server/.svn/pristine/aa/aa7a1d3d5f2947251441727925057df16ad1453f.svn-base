
function showAndHide(show,hide){
	$("#" + show).css("display","block");
	$("#" + hide).css("display","none");
}

function isContains(str, substr) {
    return new RegExp(substr).test(str);
}

function getURLQueryString(name) {
	var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)", "i");
	var r = window.location.search.substr(1).match(reg);
	if (r != null) {
		try {
			return decodeURIComponent(r[2]);
		} catch (err) {
			alert("解析错误" + err);
			return null;
		}
	}
	return null;
}

function checkSmsYzm(yzm){   
	var re=/^\d{6}$/; 
	var f = re.test(yzm);
	return f;
}

function isEmpty(str,msg){
	if(str == null || '' == str || str == '-1'){
		alert(msg);
		return true;
	}
	return false;
}

function isEmptyValue(str){
	if(str == null || '' == str || str == '-1'){
		return true;
	}
	return false;
}

function checkEmail(email,msg){
	 if (!/^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$/.test(email)) {
         alert(msg);
         return true;
     }
	return false;
}

function checkPositiveInteger(number,length){
	var numberLen = number.length;
	if(numberLen > length){
		alert("请输入长度小于" + length + "的正整数!");
		return true;
	}
	
    if (!/^[1-9]{1}[0-9]{0,8}$/.test(number)) {
	     alert("请输入合法的正整数!");
	     return true;
	}
	return false;
}

function checkPhone(d){
	if (!d) {
        alert("请输入您的手机号码");
        return false;
    } else {
        if (d && !/^1[34578]\d{9}$/.test(d)) {
            alert( "输入的手机号码不符合规范哦！");
            return false;
        } else {
        	d = d.replace(/\s/g, '');
			d = d || '';
			var repeat = 1;
			var seque = 1;
			for ( var i = 1; i < d.length; i++) {
				if (d.charAt(i) == d
						.charAt(i - 1)) {
					repeat++;
					if (repeat >= 5) {
						 alert("输入的手机号码不符合规范哦！");
				            return false;
					}
				} else {
					repeat = 1;
				}
				if (d.charAt(i)
						- d.charAt(i - 1) == '1') {
					seque++;
					if (seque >= 6) {
						 alert( "输入的手机号码不符合规范哦！");
				            return false;
					}
				} else {
					seque = 1;
				}
			}
            return true;
        }
    }
}

// 修复easyui弹窗溢出BUG
var easyuiPanelOnMove = function(left, top) {
	var parentObj = $(this).panel('panel').parent();
	if (left < 0) {
		$(this).window('move', {
			left : 1
		});
	}
	if (top < 0) {
		$(this).window('move', {
			top : 1
		});
	}
	var width = $(this).panel('options').width;
	var height = $(this).panel('options').height;
	var right = left + width;
	var buttom = top + height;
	var parentWidth = parentObj.width();
	var parentHeight = parentObj.height();
	if(parentObj.css("overflow")=="hidden"){
		if(left > parentWidth-width){
			$(this).window('move', {
				"left":parentWidth-width
			});
		}
		if(top > parentHeight-height){
			$(this).window('move', {
				"top":parentHeight-height
			});
		}
	}
};
$.fn.panel.defaults.onMove = easyuiPanelOnMove;
$.fn.window.defaults.onMove = easyuiPanelOnMove;
$.fn.dialog.defaults.onMove = easyuiPanelOnMove;

