var Namespace = {};
Namespace.create = function(namespacePath) {
	var rootObject = window;
	var namespaceParts = namespacePath.split('.');
	for (var i = 0; i < namespaceParts.length; i++) {
		var currentPart = namespaceParts[i];
		if (!rootObject[currentPart]) {
			rootObject[currentPart] = {};
		}
		rootObject = rootObject[currentPart];
	}
};

Namespace.create("util");

util.formSerializeObject = function(form) {
	var o = {};
	var form = $(form);
	var ser = form.serializeArray();
	
	$.each(ser, function(index) {
		if (o[this["name"]]) {
			o[this["name"]] = o[this["name"]] + "," + this["value"];
		} else {
			o[this["name"]] = this["value"];
		}
	});

	return o;
};

util.isAjaxMessagerProgress = false;

(function(win) {
	var console = win.console || {}, fns = [ 'assert', 'clear', 'count', 'debug', 'dir', 'dirxml', 'error', 'exception', 'group', 
	                                         'groupCollapsed', 'groupEnd', 'info', 'log', 'markTimeline', 'profile', 'profileEnd', 
	                                         'table', 'time', 'timeEnd', 'timeStamp', 'trace', 'warn' ];
	var i = 0, fn = "", ilen = fns.length;
	for (; i < ilen; i++) {
		func = fns[i];
		console[fn] = console[fn] || function() {
		};
	}
})(window);

var index = {
	centerTabs : null,		
	close : function(){
		$.messager.progress("close");
	},		
	service : {
		tabsinit : function(){
			index.centerTabs = $("#index-center-tabs").tabs();
			return true;
		},			
		addTabs : function(node){
			if(index.centerTabs.tabs("exists",node.text)){
				index.centerTabs.tabs("select",node.text)
			}else{
				if(node.attributes.url && node.attributes.url.length>0){
					index.centerTabs.tabs("add",{
						title : node.text,
						fit : true,
						closable : true,
						content : '<iframe src="'+node.attributes.url+'" name="'+node.text+'" frameborder="0" style="border:0;width:100%;height:100%"></iframe>',
						tools:[{
							iconCls : 'icon-mini-refresh',
							handler : function(){
								index.service.refreshTab(node.text);
							}
						}]
					});
				}else{
					alert("页面不存在!");
				}
			}
			return true;
		},
		refreshTab : function(title){
			var tab = index.centerTabs.tabs("getTab",title);
			index.centerTabs.tabs("update",{
				tab :  tab,
				options : tab.panel("options")
			});
			return true;
		}
	}
}

$(function() {
	if (top.index && top.index.centerTabs) {
		top.index.close();
	}

	index.service.tabsinit();
	$("#index-tree").tree({
			lines:true,
			onClick : function(node){
				if(node.children==null){
					index.service.addTabs(node);
				}				
			},			
			onDblClick : function(node){
				if(node.state == 'closed'){
					$(this).tree("expand",node.target);
				}else{
					$(this).tree("collapse",node.target);
				}
			}
	});

	$(".private-datetimebox").datetimebox({
		showSeconds : true,
		editable : false
	});

	$(".search-btn").linkbutton();
});
