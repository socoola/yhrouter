/**
 * @author Administrator
 */
$(function(){	
	$('.plus').toggle(function(event){
		if(this == event.target){
			$(this).next().show();
			$(this).css("background","url('../image/minus.gif') no-repeat left");	
		}
	}, function(event) {
		if(this == event.target){
			$(this).next().hide();
			$(this).css("background","url('../image/plus.gif') no-repeat left");
		}				
	});	
});
