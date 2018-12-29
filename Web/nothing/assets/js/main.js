(function($) {
	// three bar animation
	$("#three .bar").hover(
		function() {
			$(this).css({'marginTop': -400 + 'px'});
		},
		function() {
			$(this).css({'marginTop': 0});
		}
	);
})(jQuery);